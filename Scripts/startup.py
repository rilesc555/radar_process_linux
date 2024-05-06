import subprocess
import time
from fabric import Connection
import threading

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
    process = subprocess.Popen(['wsl', 'bash', '-c', commands], creationflags=subprocess.CREATE_NEW_CONSOLE, stdin=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
    UI = '''u'''
    
    out, err = process.communicate(input=UI)
  

# Task 3: Open a new WSL window, SSH to a Raspberry Pi, wait for the connection, input commands, and start a Python script
def startPiServer():

    session = "gimbal_control_sesh"
    with Connection("radarpi@raspberrypi.local") as c:
        c.run('source gimbal_control_env/bin/activate')
        c.run('cd gimbal')
        tmux_output = c.run(f'tmux has-session -t {session}', hide=True, warn = True)
        if not tmux_output.failed:
            print("Session already exists. Sending keys to session")
            c.run(f"tmux send-keys -t {session} 'python gimbal/gimbal_server.py' Entertm")
        else:
            print("Starting session")
            c.run(f"tmux new-session -d -s {session}")
            print("Sending keys to session")
            c.run(f"tmux send-keys -t {session} 'python gimbal/gimbal_server.py' Enter")
            


       

# Task 4: Open a program
def startUI():
    print("Task 4: Opening a program")
    command = 'program.exe'
    process = subprocess.Popen(command, shell=True)
    time.sleep(2)  # Wait for the program to open
    print("Program opened")

# Main function to run the tasks
def main():
    # proxyThread = threading.Thread(target=startProxy, name="proxy")
    processorThread = threading.Thread(target=startProcessor, name="processor")
    piServerThread = threading.Thread(target=startPiServer, name="piServer")

    # proxyThread.start()
    processorThread.start()
    piServerThread.start()

    # proxyThread.join()
    processorThread.join()
    piServerThread.join()






    
    # startProxy()
    # startProcessor()
    # startPiServer()
    # startUI()

if __name__ == '__main__':
    main()