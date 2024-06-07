
/*
import { ComboBox, Button, VerticalBox, LineEdit, ProgressIndicator, HorizontalBox } from "std-widgets.slint";
import { Page, pageination, BackToDashBoard, callbacks, properties } from "common.slint";



export component LoginPage inherits Page {
    init => {
        callbacks.get-devices()
    }
    height: 100%;
    width: 100%;
 

    VerticalBox {  
        width: root.width;
        y: 40px;
        height: parent.height / 2;
        usernamefield := LineEdit {
            height: root.height / 5;
            placeholder-text: "username";
            text: "";
        }

        passwordfield := LineEdit {
            height: root.height / 5;
            placeholder-text: "password";
            input-type: password;
            text: "";
        }

        Rectangle {
            devicebox := ComboBox {
                y: parent.height / 2;
                x: parent.width / 4;
                width: parent.width / 2;
                height: 40px;
                model: properties.Devices;
            }
        }
    }

    VerticalBox {
        width: root.width;
        y: parent.height / 3 * 2;
        height: parent.height / 5;
        loginbutton := Button {
            height: root.height / 10;
            text: "Login";
            clicked => {
                properties.currentuser.username = usernamefield.text;
                properties.currentuser.password = passwordfield.text;
                callbacks.request-login(properties.currentuser, devicebox.current-value);
                if properties.token == "" {
                    pageination.activepage=0;
                    properties.sidebaractive = true;
                    properties.sidebartest = true;
                }
                
            }   
        }

        accountbtn := Button {
            text: "Create new account";
            clicked => {
                pageination.activepage = 1;
                properties.error ="";
            }
        }
    }

}

*/