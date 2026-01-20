import socket
from kivy.app import App
from kivy.uix.boxlayout import BoxLayout
from kivy.uix.gridlayout import GridLayout
from kivy.uix.textinput import TextInput
from kivy.uix.button import Button
from kivy.config import Config

Config.set('graphics', 'width', '400')
Config.set('graphics', 'height', '600')

class CalcLayout(BoxLayout):
    def __init__(self, **kwargs):
        super().__init__(**kwargs)
        self.orientation = 'vertical'
        self.padding = 10
        self.spacing = 10

        self.display = TextInput(
            font_size=32, 
            readonly=True, 
            halign='right', 
            multiline=False,
            size_hint=(1, 0.2)
        )
        self.add_widget(self.display)

        buttons_layout = GridLayout(cols=5, spacing=10, size_hint=(1, 0.8))

        buttons = [
            'sin', 'cos', 'tan', '(', ')',
            '7', '8', '9', '/', 'C',
            '4', '5', '6', '*', '<-',
            '1', '2', '3', '-', '^',
            '0', '.', 'ln', 'log', '='
        ]

        for button_text in buttons:
            button = Button(text=button_text, font_size=24)
            button.bind(on_press=self.on_button_press)
            buttons_layout.add_widget(button)

        self.add_widget(buttons_layout)

    def on_button_press(self, instance):
        text = instance.text
        if text == '=':
            self.send_to_server()
        elif text == 'C':
            self.display.text = ''
        elif text == '<-':
            self.display.text = self.display.text[:-1]
        else:
            self.display.text += text

    def send_to_server(self):
        expression = self.display.text
        if not expression:
            return

        try:
            HOST = '127.0.0.1'
            PORT = 65433
            
            with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
                s.settimeout(3.0) 
                s.connect((HOST, PORT))
                s.sendall(expression.encode('utf-8'))
                
                data_fragments = []
                while True:
                    try:
                        chunk = s.recv(4096)
                        if not chunk:
                            break
                        data_fragments.append(chunk)
                    except socket.timeout:
                        break
                
                full_response = b''.join(data_fragments).decode('utf-8')
                
            self.display.text = full_response
            
        except ConnectionRefusedError:
            self.display.text = "Connection Refused"
        except Exception as e:
            self.display.text = f"Error: {str(e)}"

class ScientificCalcApp(App):
    def build(self):
        return CalcLayout()

if __name__ == '__main__':
    ScientificCalcApp().run()