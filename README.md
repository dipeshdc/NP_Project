# Remote Command Executor

## How to Run

### 1. Compile the code

```bash
gcc server.c -o server
gcc client.c -o client
```

### 2. Run the Server

```bash
./server <port>
```

Example:

```bash
./server 9999
```

### 3. Run the Client

```bash
./client <server_ip> <port>
```

Example:

```bash
./client 127.0.0.1 9999
```

### 4. Usage

- After connection, type shell commands into the client.
- Type `exit` to close the connection.
