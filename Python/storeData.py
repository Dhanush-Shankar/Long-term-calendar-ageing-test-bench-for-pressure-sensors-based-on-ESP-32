
import constants
import psycopg2
import psycopg2.extras


def store_in_database(sensordata):
    
    data = ""
    for i in range(len(sensordata)):
        if sensordata[i] is None:
            sensordata[i] = "NULL"
    data = ",".join(sensordata)
    # print(data)
    try:
        with psycopg2.connect(
                host=constants.hostname_DB,
                dbname=constants.database_DB,
                user=constants.username_DB,
                password=constants.pwd_DB,
                port=constants.port_id_DB) as conn:

            with conn.cursor(cursor_factory=psycopg2.extras.DictCursor) as cur:

                cur.execute(constants.create_table_script)
                insert_script = 'INSERT INTO sensordatatest1 (timestamp, LoadCell ,Sensor1, Sensor2, Sensor3,Sensor4,Sensor5,Sensor6,Sensor7,Sensor8,Sensor9,Sensor10,Sensor11,Sensor12,Sensor13,Sensor14,Sensor15) VALUES (now(),' + data + ');'
                print(insert_script)
                # cur.execute(insert_script)
                print("Data Stored")
    except Exception as error:
        print(error)
    finally:
        if conn is not None:
            conn.close()
