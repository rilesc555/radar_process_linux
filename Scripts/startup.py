import subprocess
import time

# Function to execute a command and return the output
def execute_command(command):
    process = subprocess.Popen(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True)
    output, error = process.communicate()
    return output.decode('utf-8'), error.decode('utf-8')

# Task 1: Open a WSL window, start a Python script in sudo mode, and input the admin password
def startProxy():
    
    commands = "cd; echo 'cuav' | sudo -S python /mnt/c/Users/CUAV/Documents/Projects/radar_process_linux/Scripts/proxy.py"
    subprocess.Popen(['wsl', 'bash', '-c', commands], creationflags=subprocess.CREATE_NEW_CONSOLE)


# Task 2: Open a new WSL window, start a program, input commands, and keep it running
def startProcessor():
    commands = '''cd 
    /home/cuav/.vs/radar_process_linux/out/build/linux-release/radar_process
    exec bash'''
    process = subprocess.Popen(['wsl', 'bash', '-c', commands], creationflags=subprocess.CREATE_NEW_CONSOLE, stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE,)
    commands = '''u'''
    out, err = process.communicate(input=commands)
    print(out)
    if err:
        print(err)
  

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
    startProcessor()
    # startPiServer()
    # startUI()

if __name__ == '__main__':
    main()