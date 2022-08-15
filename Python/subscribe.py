import time
import json
import paho.mqtt.subscribe as subscribe
import psycopg2
import psycopg2.extras
import constants
from storeData import store_in_database

loop_forever = True
field = "field"
# &field4=23&field5=23&field6=23&field7=23&field8=23

while loop_forever:
    try:
        print("Subscribing to Topic: ", constants.topic)
        msg = subscribe.simple(constants.topic, hostname=constants.hostname, client_id=constants.client_id,
                            port=constants.port, tls=constants.tls, transport=constants.transport, auth=constants.auth)

        data = json.loads(msg.payload.decode('ascii'))
        sensordata = []
        for i in range(1, constants.total_sensor_count):
            s = str(i)
            try:
                sensordata.append(data[field+s])
            except KeyError:
                x =  len(sensordata)
                for l in range(x,(constants.total_sensor_count-1)):
                    sensordata.append(None)

        #print(sensordata)
        store_in_database(sensordata)

        time.sleep(5)

    except KeyboardInterrupt:
        loop_forever = False
        print("Program Stopped by keyboard Interrupt")
