
total_sensor_count = 17

#Credentials for MQTT Broker
topic = "your things speak channel name"
hostname = "mqtt3.thingspeak.com"
client_id = "client_id"
port = 1883
tls = None
transport = "tcp"
auth = {'username': "username",
        'password': "password"}

#credentials for Postgres DB
hostname_DB = ''
database_DB = ''
username_DB = ''
pwd_DB = ''
port_id_DB = 5432

create_table_script = ''' CREATE TABLE IF NOT EXISTS  sensordatatest1(
                                                id      SERIAL  PRIMARY KEY,
                                                timestamp TIMESTAMP,
                                                LoadCell int DEFAULT NULL,
                                                Sensor1 int DEFAULT NULL,
                                                Sensor2 int DEFAULT NULL,
                                                Sensor3 int DEFAULT NULL,
                                                Sensor4 int DEFAULT NULL,
                                                Sensor5 int DEFAULT NULL,
                                                Sensor6 int DEFAULT NULL,
                                                Sensor7 int DEFAULT NULL,
                                                Sensor8 int DEFAULT NULL,
                                                Sensor9 int DEFAULT NULL,
                                                Sensor10 int DEFAULT NULL,
                                                Sensor11 int DEFAULT NULL,
                                                Sensor12 int DEFAULT NULL,
                                                Sensor13 int DEFAULT NULL,
                                                Sensor14 int DEFAULT NULL,
                                                Sensor15 int DEFAULT NULL
                                                
                                                ) '''
