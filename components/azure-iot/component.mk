#
# Component Makefile
#
#COMPONENT_ADD_INCLUDEDIRS := sdk/iothub_client/inc sdk/c-utility/inc sdk/uamqp/inc 

#COMPONENT_SRCDIRS := sdk/iothub_client/src sdk/c-utility/src sdk/uamqp/src

COMPONENT_ADD_INCLUDEDIRS :=  \
sdk/c-utility/inc  \
sdk/c-utility/inc/azure_c_shared_utility \
sdk/iothub_client/inc \
sdk/umqtt/inc  \
sdk/umqtt/inc/azure_umqtt_c 	\
sdk/parson

COMPONENT_OBJS =  \
sdk/c-utility/src/xlogging.o	\
sdk/c-utility/src/buffer.o	\
sdk/c-utility/src/consolelogger.o	\
sdk/c-utility/src/constbuffer.o	\
sdk/c-utility/src/constmap.o	\
sdk/c-utility/src/crt_abstractions.o	\
sdk/c-utility/src/doublylinkedlist.o	\
sdk/c-utility/src/gballoc.o	\
sdk/c-utility/src/gb_stdio.o	\
sdk/c-utility/src/gb_time.o	\
sdk/c-utility/src/hmac.o	\
sdk/c-utility/src/hmacsha256.o	\
sdk/c-utility/src/httpapiex.o	\
sdk/c-utility/src/httpapiexsas.o	\
sdk/c-utility/src/httpheaders.o	\
sdk/c-utility/src/map.o	\
sdk/c-utility/src/optionhandler.o	\
sdk/c-utility/src/sastoken.o	\
sdk/c-utility/src/sha1.o	\
sdk/c-utility/src/sha224.o	\
sdk/c-utility/src/sha384-512.o	\
sdk/c-utility/src/strings.o	\
sdk/c-utility/src/string_tokenizer.o	\
sdk/c-utility/src/urlencode.o	\
sdk/c-utility/src/usha.o	\
sdk/c-utility/src/vector.o	\
sdk/c-utility/src/xio.o	\
sdk/c-utility/src/base64.o \
\
\
sdk/iothub_client/src/iothub_client.o	\
sdk/iothub_client/src/iothub_client_ll.o	\
sdk/iothub_client/src/iothub_client_ll_uploadtoblob.o	\
sdk/iothub_client/src/iothub_message.o	\
sdk/iothub_client/src/iothubtransport.o	\
sdk/iothub_client/src/iothubtransportmqtt.o	\
sdk/iothub_client/src/iothubtransport_mqtt_common.o	\
sdk/iothub_client/src/version.o	\
\
\
sdk/umqtt/src/mqtt_client.o	\
sdk/umqtt/src/mqtt_codec.o	\
sdk/umqtt/src/mqtt_message.o	\
\
\
\
sdk/c-utility/adapters/bad-adapters/agenttime_esp32.o	\
sdk/c-utility/adapters/bad-adapters/platform_esp32.o	\
sdk/c-utility/adapters/bad-adapters/threadapi_esp32.o	\
sdk/c-utility/adapters/tickcounter_freertos.o	\
sdk/c-utility/adapters/bad-adapters/tlsio_ssl_esp32.o	

COMPONENT_SRCDIRS :=  \
sdk/c-utility/src \
sdk/c-utility/adapters  \
sdk/c-utility/adapters/bad-adapters  \
sdk/umqtt/src	\
sdk/iothub_client/src  \
sdk/parson	\















