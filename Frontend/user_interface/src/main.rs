use std::{collections::HashMap, env};
use reqwest::{Error, Response};
use serde::{Deserialize, Serialize};

slint::include_modules!();



#[derive(Serialize,Deserialize)]
struct ServerResponse {
    message: String,
}

fn main() -> Result<(), slint::PlatformError> {
    let ui = AppWindow::new()?;
    //ui.global::<callbacks>().on_request_login
    ui.on_login({
        move |data: Credentials| {
            // let tokio_runtime = tokio::runtime::Runtime::new().unwrap();
            // slint::spawn_local( async move {

            // })
            //should send username and hashed password to api at {API-CON-STRING}
            //after awaiting and recieving a response it should either log in and open up the dashboard
            //or an error message should pop up asking the user to try again or create a new account

        }
    });
    // create_user(data);
    //It should take a username, and password, where it checks whether the first password is the same
    //should check if the data is proper
    //as the second after which it should send a post request to api and then "redirect" to login page
    //while displaying a confirmation of some sort.
    ui.on_createacc({
        move |data: Credentials| {
            print!("{data:?}");
            //needs to use slint::spawn_local()
            //need to figure out how to use said functionality
            //creates a new user in the database
            let tokio_runtime = tokio::runtime::Runtime::new().unwrap();
            slint::spawn_local( async move {
                    let res: Result<Response,Error>  = tokio_runtime.spawn(async move {
                        // This code is running on the Tokio runtime's thread, you can await futures that depends on Tokio here.
                        let mut map = HashMap::new();
                        map.insert("username", data.username.to_string());
                        map.insert("password", data.password.to_string());
                        let client = reqwest::Client::new();
                        // fn test() -> String{
                        //     return format!(
                        //         "{}{}",
                        //         env::var("DB_CON").unwrap(), "createUser"
                        //     )
                        // }
                        let response = client.post("https://h3-projektv2-24q2h3-gruppe3-txp.onrender.com/createUser").json(&map).send().await;
                        return response;
                    }).await.unwrap(); 
                    if res.is_ok() {
                        //ui.set_usercreated(true)

                    }
                    
                   //let result = from_str::<server_response>(result);
                }).unwrap();     
            }
        });
        
        ui.run()
    }



