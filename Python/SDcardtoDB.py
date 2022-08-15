from tkinter import Tk     # from tkinter import Tk for Python 3.x
from tkinter.filedialog import askopenfilename
import constants
import psycopg2
import psycopg2.extras
import csv

Tk().withdraw()  
filename = askopenfilename()

with open(filename, 'r') as file:
    reader = csv.reader(file)
    next(reader) #skips the header

    for row in reader:


        try:
            with psycopg2.connect(
                host=constants.hostname_DB,
                dbname=constants.database_DB,
                user=constants.username_DB,
                password=constants.pwd_DB,
                port=constants.port_id_DB) as conn:

                with conn.cursor(cursor_factory=psycopg2.extras.DictCursor) as cur:

                    create_script = 'SELECT timestamp FROM sensordatatest1 WHERE timestamp = ' + '\'' + row[1] + '\'' + ';'
                    data = ",".join(row)
                                         

                    cur.execute(create_script)
                    if cur.rowcount == 0:#checks if  data of given time stamp exist already or not
                        insert_script = 'INSERT INTO sensordatatest1 (timestamp, LoadCell ,Sensor1, Sensor2, Sensor3,Sensor4,Sensor5,Sensor6,\
                        Sensor7,Sensor8,Sensor9,Sensor10,Sensor11,\
                        Sensor12,Sensor13,Sensor14,Sensor15)   \
                        VALUES (''\'' + row[1] + '\',' + \
                            row[2] + ',' + row[3] + ',' + \
                            row[4] + ',' + row[5] + ',' + \
                            row[6] + ',' + row[7] + ',' + \
                            row[8] + ',' + row[9] + ',' + \
                            row[10] + ',' + row[11] + ',' + \
                            row[12] + ',' + row[13] + ',' + \
                            row[14] + ',' + row[15] + ',' + \
                            row[15] + ',' + row[16] + ');'

                        cur.execute(insert_script)
                        print("Data Stored")

        except Exception as error:
            print(error)
        finally:
            if conn is not None:
                conn.close()
