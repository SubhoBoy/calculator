import socket
from kivy.app import App
from kivy.uix.boxlayout import BoxLayout
from kivy.uix.textinput import TextInput
from kivy.uix.button import Button
from kivy.uix.label import Label
from kivy.uix.scrollview import ScrollView  # Added for scrolling large results

class CalcLayout(BoxLayout):
    def __init__(self, **kwargs):
        super().__init__(**kwargs)
        self.orientation = 'vertical'
        self.padding = 20
        self.spacing = 10

        # Input Area
        self.expression_input = TextInput(
            hint_text='Enter Expression (e.g. sin(x) or 5*5)', 
            multiline=False, 
            size_hint=(1, 0.15),
            font_size=24
        )
        self.add_widget(self.expression_input)

        # Calculate Button
        self.calc_button = Button(
            text='Compute', 
            size_hint=(1, 0.15),
            background_color=(0.2, 0.6, 0.8, 1)
        )
        self.calc_button.bind(on_press=self.send_to_server)
        self.add_widget(self.calc_button)

        # Result Display (Wrapped in ScrollView for large datasets)
        self.result_scroll = ScrollView(size_hint=(1, 0.7))
        self.result_label = Label(
            text='Ready', 
            size_hint_y=None,
            font_size=18,
            halign="left",
            valign="top"
        )
        # Magic to make the label grow with text inside ScrollView
        self.result_label.bind(texture_size=self.result_label.setter('size'))
        self.result_label.bind(width=lambda *x: self.result_label.setter('text_size')(self.result_label, (self.result_label.width, None)))
        
        self.result_scroll.add_widget(self.result_label)
        self.add_widget(self.result_scroll)

    def send_to_server(self, instance):
        expression = self.expression_input.text
        if not expression:
            self.result_label.text = "Please enter an expression."
            return

        self.result_label.text = "Calculating..."
        
        try:
            HOST = '127.0.0.1'
            PORT = 65433  # CORRECTION: Matches server.py port
            
            with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
                s.settimeout(3.0) 
                s.connect((HOST, PORT))
                s.sendall(expression.encode('utf-8'))
                
                # CORRECTION: Loop to receive all data (chunks)
                data_fragments = []
                while True:
                    try:
                        chunk = s.recv(4096)
                        if not chunk:
                            break
                        data_fragments.append(chunk)
                    except socket.timeout:
                        break # Stop if server hangs, though server should close conn
                
                full_response = b''.join(data_fragments).decode('utf-8')
                
            # Basic formatting for the user
            if len(full_response) > 500:
                header = "Large Dataset Received (Truncated for View):\n"
                self.result_label.text = header + full_response[:1000] + "\n...[more data]..."
            else:
                self.result_label.text = f"Result:\n{full_response}"
            
        except ConnectionRefusedError:
            self.result_label.text = "Error: Connection Refused.\nEnsure server.py is running on Port 65433."
        except Exception as e:
            self.result_label.text = f"Error: {str(e)}"

class ScientificCalcApp(App):
    def build(self):
        return CalcLayout()

if __name__ == '__main__':
    ScientificCalcApp().run()
