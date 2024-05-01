import subprocess
import time

# Function to execute a command and return the output
def execute_command(command):
    process = subprocess.Popen(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True)
    output, error = process.communicate()
    return output.decode('utf-8'), error.decode('utf-8')

# Task 1: Open a WSL window, start a Python script in sudo mode, and input the admin password
def startProxy():
    print("Task 1: Running Python script in WSL with sudo")
    command = 'wsl echo "password" | sudo -S python3 script.py'
    output, error = execute_command(command)
    print("Output:", output)
    print("Error:", error)

# Task 2: Open a new WSL window, start a program, input commands, and keep it running
def startProcessor():
    print("Task 2: Starting a program in WSL and keeping it running")
    command = 'wsl bash -c "echo commands | program"'
    process = subprocess.Popen(command, shell=True)
    time.sleep(5)  # Wait for the program to start
    print("Program is running in the background")

# Task 3: Open a new WSL window, SSH to a Raspberry Pi, wait for the connection, input commands, and start a Python script
def startPiServer():
    print("Task 3: SSH to Raspberry Pi, input commands, and start a Python script")
    command = 'wsl bash -c "ssh pi@raspberry_pi_ip \'echo commands; python3 script.py\'"'
    output, error = execute_command(command)
    print("Output:", output)
    print("Error:", error)

# Task 4: Open a program
def startUI():
    print("Task 4: Opening a program")
    command = 'program.exe'
    process = subprocess.Popen(command, shell=True)
    time.sleep(2)  # Wait for the program to open
    print("Program opened")

# Main function to run the tasks
def main():
    # startProxy()
    # startProcessor()
    # startPiServer()
    # startUI()

    subprocess.run(["wsl", "sudo python /mnt/c/Users/CUAV/Documents/Projects/radar_process_linux/Scripts/proxy.py", "cuav", "exec bash"], shell=True)

if __name__ == '__main__':
    main()