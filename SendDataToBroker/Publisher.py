import datetime
import paho.mqtt.client as mqtt
import json
import time
import sqlite3
import threading

# MQTT Broker details
BROKER = "broker.emqx.io"
PORT = 1883
TOPIC = "Employees Es4031/topic"

# Create SQLite database connection and cursor
# Enabled check_same_thread=False for multi-threaded access
conn = sqlite3.connect("Employees.db", check_same_thread=False)
cur = conn.cursor()

# Create table if it doesn't exist
cur.execute('''
CREATE TABLE IF NOT EXISTS employees (
    id INTEGER PRIMARY KEY,
    name TEXT NOT NULL,
    enter_time TEXT NOT NULL,
    exit_time TEXT,
    status TEXT NOT NULL,
    elapsed_time INTEGER DEFAULT 0
)
''')
conn.commit()

# MQTT Client Setup
client = mqtt.Client()

def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("Connected to MQTT Broker!")
        client.subscribe(TOPIC)
        print(f"Subscribed to topic: {TOPIC}")
    else:
        print(f"Failed to connect, return code {rc}")

client.on_connect = on_connect

def add_to_db(payload):
    try:
        # Use INSERT OR REPLACE to handle unique constraint on 'id'
        cur.execute('''
        INSERT OR REPLACE INTO employees (id, name, enter_time, exit_time, status, elapsed_time)
        VALUES (?, ?, ?, ?, ?, ?)
        ''', (
            payload["ID"],
            payload["name"],
            payload["enter_time"],
            payload.get("exit_time"),
            payload["status"],
            payload.get("elapsed_time", 0)
        ))
        conn.commit()
    except sqlite3.Error as e:
        print(f"SQLite error: {e}")

def update_status_to_idle(employee_id):
    # Wait for 5 seconds before setting status to 'Idle'
    time.sleep(5)
    try:
        # Retrieve enter_time to calculate elapsed_time
        cur.execute('SELECT enter_time FROM employees WHERE id = ?', (employee_id,))
        row = cur.fetchone()
        if row and row[0]:
            enter_time_str = row[0]
            enter_time = datetime.datetime.strptime(enter_time_str, "%H:%M:%S")
            current_time = datetime.datetime.now()
            elapsed_time = int((current_time - enter_time).total_seconds())
            exit_time_str = current_time.strftime("%Y-%m-%d %H:%M:%S")
            
            # Update status to 'Idle', set exit_time and elapsed_time
            cur.execute('''
            UPDATE employees
            SET status = ?, exit_time = ?, elapsed_time = ?
            WHERE id = ?
            ''', ("Idle", exit_time_str, elapsed_time, employee_id))
            conn.commit()

            # Publish the updated status
            cur.execute('SELECT * FROM employees WHERE id = ?', (employee_id,))
            updated_row = cur.fetchone()
            if updated_row:
                data = {
                    "ID": updated_row[0],
                    "name": updated_row[1],
                    "enter_time": updated_row[2],
                    "exit_time": updated_row[3],
                    "status": updated_row[4],
                    "elapsed_time": updated_row[5]
                }
                json_data = json.dumps(data)    
                print(f"Sending updated data: {json_data}")
                client.publish(TOPIC, json_data)
        else:
            print(f"No enter_time found for employee ID {employee_id}")
    except sqlite3.Error as e:
        print(f"SQLite error during status update: {e}")
    except Exception as ex:
        print(f"Error in update_status_to_idle: {ex}")

def publish_employee_data(employee):
    try:
        json_data = json.dumps(employee)
        print(f"Sending data: {json_data}")
        client.publish(TOPIC, json_data)
    except Exception as e:
        print(f"Error publishing data: {e}")

def simulate_attendance():
    # Example employees data
    employees_data = [
        {
            "ID": 0,
            "name": "Unknown",
            "status": "Failed"
        },
        {
            "ID": 1,
            "name": "Amirhosein",
            "status": "Success"
        },
        {
            "ID": 2,
            "name": "Ali",
            "status": "Success"
        }
    ]

    for emp in employees_data:
        current_time = datetime.datetime.now().strftime("%H:%M:%S")
        payload = {
            "ID": emp["ID"],
            "name": emp["name"],
            "enter_time": current_time,
            "exit_time": None,
            "status": emp["status"],
            "elapsed_time": 0  # Initial elapsed_time
        }
        add_to_db(payload)
        publish_employee_data(payload)
        
        # Start a thread to update status to 'Idle' after 5 seconds
        threading.Thread(target=update_status_to_idle, args=(emp["ID"],)).start()
        
        time.sleep(2)  # Simulate time between attendances

def main():
    try:
        client.connect(BROKER, PORT, keepalive=60)
        client.loop_start()
        time.sleep(2)  # Wait for connection
        
        simulate_attendance()
        
        # Keep the script running to handle idle updates
        while True:
            time.sleep(60)
            
    except Exception as e:
        print(f"An error occurred: {e}")
    finally:
        # Clean up and disconnect
        input("Press Enter to exit...\n")
        client.loop_stop()
        client.disconnect()
        conn.close()

if __name__ == "__main__":
    main()
