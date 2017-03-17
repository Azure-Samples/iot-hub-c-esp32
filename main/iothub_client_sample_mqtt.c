// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <stdio.h>
#include <stdlib.h>

#include "iothub_client.h"
#include "iothub_message.h"
#include "azure_c_shared_utility/threadapi.h"
#include "azure_c_shared_utility/crt_abstractions.h"
#include "azure_c_shared_utility/platform.h"
#include "iothubtransportmqtt.h"

#include "azure_c_shared_utility/tickcounter.h"

#ifdef MBED_BUILD_TIMESTAMP
#include "certs.h"
#endif // MBED_BUILD_TIMESTAMP

/*String containing Hostname, Device Id & Device Key in the format:                         */
/*  "HostName=<host_name>;DeviceId=<device_id>;SharedAccessKey=<device_key>"                */
/*  "HostName=<host_name>;DeviceId=<device_id>;SharedAccessSignature=<device_sas_token>"    */
static const char* connectionString = "";

static int callbackCounter;
static char msgText[1024];
static char propText[1024];
static bool g_continueRunning;
#define MESSAGE_COUNT 5
#define DOWORK_LOOP_NUM     3

#if RUN_TIMER_TEST
static bool doRunTimerTest = true;
// Timer test
static void RunTimerTest()
{
	time_t timer;
	char buffer[32];
	struct tm* tm_info;
	tickcounter_ms_t ticks = 0;

	(void)printf("Running timer test...\r\n");
	TICK_COUNTER_HANDLE th = tickcounter_create();
	if (th != NULL)
	{
		for (int i = 0; i < 6; i++)
		{
			time(&timer);
			tm_info = localtime(&timer);

			strftime(buffer, 32, "%Y-%m-%d %H:%M:%S\r\n", tm_info);
			(void)printf(buffer);
			tickcounter_get_current_ms(th, &ticks);
			(void)printf("ticks: %u\r\n", ticks);
			ThreadAPI_Sleep(1000);
		}
		tickcounter_destroy(th);
	}
	else
	{
		(void)printf("Failed timer test due to NULL tickcounter_create.\r\n");
	}
}
#endif


typedef struct EVENT_INSTANCE_TAG
{
    IOTHUB_MESSAGE_HANDLE messageHandle;
    size_t messageTrackingId;  // For tracking the messages within the user callback.
} EVENT_INSTANCE;

static unsigned char* bytearray_to_str(const unsigned char *buffer, size_t len)
{
    unsigned char* ret = (unsigned char*)malloc(len+1);
    memcpy(ret, buffer, len);
    ret[len] = '\0';
    return ret;
}

static IOTHUBMESSAGE_DISPOSITION_RESULT ReceiveMessageCallback(IOTHUB_MESSAGE_HANDLE message, void* userContextCallback)
{

    int* counter = (int*)userContextCallback;
    const char* buffer;
    size_t size;

    if (IoTHubMessage_GetByteArray(message, (const unsigned char**)&buffer, &size) != IOTHUB_MESSAGE_OK)
    {
        (void)printf("unable to retrieve the message data\r\n");
    }
    else
    {
        unsigned char* message_string = bytearray_to_str((const unsigned char *)buffer, size);
        (void)printf("IoTHubMessage_GetByteArray received message: \"%s\" \n", message_string);
        free(message_string);

        // If we receive the word 'quit' then we stop running
        if (size == (strlen("quit") * sizeof(char)) && memcmp(buffer, "quit", size) == 0)
        {
            g_continueRunning = false;
        }
    }

    // Retrieve properties from the message
    MAP_HANDLE mapProperties = IoTHubMessage_Properties(message);
    if (mapProperties != NULL)
    {
        const char*const* keys;
        const char*const* values;
        size_t propertyCount = 0;
        if (Map_GetInternals(mapProperties, &keys, &values, &propertyCount) == MAP_OK)
        {
            if (propertyCount > 0)
            {
                size_t index = 0;
                for (index = 0; index < propertyCount; index++)
                {
                    //(void)printf("\tKey: %s Value: %s\r\n", keys[index], values[index]);
                }
                //(void)printf("\r\n");
            }
        }
    }

    /* Some device specific action code goes here... */
    (*counter)++;
    return IOTHUBMESSAGE_ACCEPTED;
}

static void SendConfirmationCallback(IOTHUB_CLIENT_CONFIRMATION_RESULT result, void* userContextCallback)
{
    EVENT_INSTANCE* eventInstance = (EVENT_INSTANCE*)userContextCallback;
    size_t id = eventInstance->messageTrackingId;

    (void)printf("Confirmation[%d] received for message tracking id = %d with result = %s\r\n", callbackCounter, (int)id, ENUM_TO_STRING(IOTHUB_CLIENT_CONFIRMATION_RESULT, result));
    /* Some device specific action code goes here... */
    callbackCounter++;
    IoTHubMessage_Destroy(eventInstance->messageHandle);
}

void iothub_client_sample_mqtt_run(void)
{
	printf("\nFile:%s Compile Time:%s %s\n",__FILE__,__DATE__,__TIME__);
    //(void)printf("size after iothub_client_sample_mqtt_run starts: %d \n", system_get_free_heap_size());
    IOTHUB_CLIENT_LL_HANDLE iotHubClientHandle;

    EVENT_INSTANCE messages[MESSAGE_COUNT];

    g_continueRunning = true;
    srand((unsigned int)time(NULL));
    double avgWindSpeed = 10.0;

    callbackCounter = 0;
    int receiveContext = 0;
    //(void)printf("size before platform_init: %d \n", system_get_free_heap_size());
    if (platform_init() != 0)
    {
        (void)printf("Failed to initialize the platform.\r\n");
    }
    else
    {
        //(void)printf("size before IoTHubClient_LL_CreateFromConnectionString: %d \n", system_get_free_heap_size());
        if ((iotHubClientHandle = IoTHubClient_LL_CreateFromConnectionString(connectionString, MQTT_Protocol)) == NULL)
        {
            (void)printf("ERROR: iotHubClientHandle is NULL!\r\n");
        }
        else
        {
            bool traceOn = true;
            //(void)printf("size before IoTHubClient_LL_SetOption: %d \n", system_get_free_heap_size());
            IoTHubClient_LL_SetOption(iotHubClientHandle, "logtrace", &traceOn);

#ifdef MBED_BUILD_TIMESTAMP
            // For mbed add the certificate information
            if (IoTHubClient_LL_SetOption(iotHubClientHandle, "TrustedCerts", certificates) != IOTHUB_CLIENT_OK)
            {
                printf("failure to set option \"TrustedCerts\"\r\n");
            }
#endif // MBED_BUILD_TIMESTAMP

            /* Setting Message call back, so we can receive Commands. */
            //(void)printf("size before IoTHubClient_LL_SetMessageCallback: %d \n", system_get_free_heap_size());
            if (IoTHubClient_LL_SetMessageCallback(iotHubClientHandle, ReceiveMessageCallback, &receiveContext) != IOTHUB_CLIENT_OK)
            {
                (void)printf("ERROR: IoTHubClient_LL_SetMessageCallback..........FAILED!\r\n");
            }
            else
            {
                (void)printf("IoTHubClient_LL_SetMessageCallback...successful.\r\n");

                /* Now that we are ready to receive commands, let's send some messages */
                size_t iterator = 0;

                do
                {
                    if (iterator < MESSAGE_COUNT && (iterator<= callbackCounter))
                    {
                        sprintf_s(msgText, sizeof(msgText), "{\"deviceId\":\"AirConditionDevice_001\",\"windSpeed\":%.2f}", avgWindSpeed + (rand() % 4 + 2));
                        printf("Ready to Send String:%s\n",(const char*)msgText);
                        //(void)printf("size before IoTHubMessage_CreateFromByteArray: %d \n", system_get_free_heap_size());
                        if ((messages[iterator].messageHandle = IoTHubMessage_CreateFromByteArray((const unsigned char*)msgText, strlen(msgText))) == NULL)
                        {
                            (void)printf("ERROR: iotHubMessageHandle is NULL!\r\n");
                        }
                        else
                        {
                            messages[iterator].messageTrackingId = iterator;
                            //(void)printf("size before IoTHubMessage_Properties: %d \n", system_get_free_heap_size());
                            MAP_HANDLE propMap = IoTHubMessage_Properties(messages[iterator].messageHandle);
                            (void)sprintf_s(propText, sizeof(propText), "PropMsg_%zu", iterator);
                            if (Map_AddOrUpdate(propMap, "PropName", propText) != MAP_OK)
                            {
                                (void)printf("ERROR: Map_AddOrUpdate Failed!\r\n");
                            }
                            //(void)printf("free heap size before IoTHubClient_LL_SendEventAsync: %d \n", system_get_free_heap_size());
                            if (IoTHubClient_LL_SendEventAsync(iotHubClientHandle, messages[iterator].messageHandle, SendConfirmationCallback, &messages[iterator]) != IOTHUB_CLIENT_OK)
                            {
                                (void)printf("ERROR: IoTHubClient_LL_SendEventAsync..........FAILED!\r\n");
                            }
                            else
                            {
                                (void)printf("IoTHubClient_LL_SendEventAsync accepted message [%d] for transmission to IoT Hub.\r\n", (int)iterator);
                            }
                        }
                        iterator++;
                    }
                    IoTHubClient_LL_DoWork(iotHubClientHandle);
                    ThreadAPI_Sleep(1);

					if (doRunTimerTest)
					{
						doRunTimerTest = false;
#if RUN_TIMER_TEST

						RunTimerTest();
#endif
					}
                    // if (callbackCounter>=MESSAGE_COUNT){
                    //     printf("done sending...\n");
                    //     break;
                    // }
                } while (g_continueRunning);

                (void)printf("iothub_client_sample_mqtt has gotten quit message, call DoWork %d more time to complete final sending...\r\n", DOWORK_LOOP_NUM);
                size_t index = 0;
                for (index = 0; index < DOWORK_LOOP_NUM; index++)
                {
                    IoTHubClient_LL_DoWork(iotHubClientHandle);
                    ThreadAPI_Sleep(1);
                }
            }
            IoTHubClient_LL_Destroy(iotHubClientHandle);
        }
        platform_deinit();
    }
}

int main(void)
{
    iothub_client_sample_mqtt_run();
    return 0;
}
