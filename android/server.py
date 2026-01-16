import socket
import ctypes
import os

# Load library
# Ensure func.c and pars2.c are compiled into libcalc.so:
# gcc -shared -o libcalc.so -fPIC func.c pars2.c
so_file = os.path.abspath("libcalc.so") 
lib = ctypes.CDLL(so_file)

# Define C function signature
# void compute_expression(const char* input, double start, double end, double step, char* output)
lib.compute_expression.argtypes = [
    ctypes.c_char_p, 
    ctypes.c_double, 
    ctypes.c_double, 
    ctypes.c_double, 
    ctypes.c_char_p
]
lib.compute_expression.restype = None

def process_request(expression):
    expression = expression.strip()
    
    # Simple heuristic: if 'x' or 't' is present, it's a plot request.
    # Otherwise, it's a simple calculation.
    is_plot = 'x' in expression or 't' in expression
    
    # Prepare buffers
    input_str = expression.encode('utf-8')
    # Increased buffer size for plotting data
    output_buffer = ctypes.create_string_buffer(65536) 
    
    if is_plot:
        # Plot mode: 0 to 10 with step 0.5
        # (Users can change this logic to accept params from client if needed)
        lib.compute_expression(input_str, 0.0, 10.0, 0.5, output_buffer)
    else:
        # Scalar mode: Range doesn't matter, step=0 triggers single eval
        lib.compute_expression(input_str, 0.0, 0.0, 0.0, output_buffer)
    
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
            try:
                conn, addr = s.accept()
                with conn:
                    #print(f"[Connected] {addr}")
                    data = conn.recv(1024)
                    if not data:
                        break
                    expr = data.decode('utf-8')
                    print(f"[Received] {expr}")
                    
                    try:
                        response = process_request(expr)
                    except Exception as e:
                        response = f"Error: {e}".encode('utf-8')
                    
                    conn.sendall(response)
            except KeyboardInterrupt:
                print("\nStopping server...")
                break
            except Exception as e:
                print(f"Server Error: {e}")

if __name__ == "__main__":
    start()
