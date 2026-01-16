import socket
import ctypes
import os

so_file = os.path.abspath("libcalc.so") # abs path cuz otherwise termx blerghs
lib = ctypes.CDLL(so_file)

# Define C function signature
# void compute_expression(const char* input, double start, double end, double step, char* output)
lib.compute_expression.argtypes = [ctypes.c_char_p, ctypes.c_double, ctypes.c_double, ctypes.c_double, ctypes.c_char_p]
lib.compute_expression.restype = None

def process_request(expression):
    # Prepare buffers
    input_str = expression.encode('utf-8')
    output_buffer = ctypes.create_string_buffer(1024)
    
    # Call C Logic (Example params: t=0 to 10, step=0.1)
    lib.compute_expression(input_str, 0.0, 10.0, 0.1, output_buffer)
    
    return output_buffer.value

HOST = '127.0.0.1'
PORT = 65433

def start():
    print(f"[*] C-Calc Core running on {HOST}:{PORT}")
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        s.bind((HOST, PORT))
        s.listen()
        
        while True:
            conn, addr = s.accept()
            with conn:
                data = conn.recv(1024)
                if not data:
                    break
                expr = data.decode('utf-8')
                print(f"[Received] {expr}")
                
                # Process with C
                response = process_request(expr)
                
                conn.sendall(response)

if __name__ == "__main__":
    start()
