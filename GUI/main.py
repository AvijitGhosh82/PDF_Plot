import os
import kivy
# kivy.require('1.8.0')

from kivy.app import App
from kivy.uix.boxlayout import BoxLayout
from kivy.properties import ObjectProperty

class AppRoot(BoxLayout):
    img_picker = ObjectProperty(None)

    def __init__(self, *args, **kwargs):
        super(AppRoot, self).__init__(*args, **kwargs)
        self.img_picker = ImagePicker()

    def change_to_img_picker(self):
        self.clear_widgets()
        self.add_widget(self.img_picker)

    # def change_to_about_us(self):
    #     print "change_to_about_us"

class ImagePicker(BoxLayout):
    def __init__(self, *args, **kwargs):
        super(ImagePicker, self).__init__(*args, **kwargs)

    def open(self, path, selection):
        print "OPEN", selection[0]
        os.system("python pdfwidget.py "+selection[0])
        

    def selected(self, selection):
        print "SELECTION"

class GraphParserApp(App):
    def build(self):
        return AppRoot()

if __name__ == "__main__":
    GraphParserApp().run()