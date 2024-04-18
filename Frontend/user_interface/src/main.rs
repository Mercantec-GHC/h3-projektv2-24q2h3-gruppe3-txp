use std::{collections::HashMap, env};
use reqwest::{Client, Error, Response};
use slint::platform::Key;
use slint::{EventLoopError, JoinHandle};
use tokio::runtime;
slint::include_modules!();
// use std::thread;
// use std::time::Duration;

// fn main() {
//     thread::spawn(|| {
//         for i in 1..10 {
//             println!("hi number {} from the spawned thread!", i);
//             thread::sleep(Duration::from_millis(1));
//         }
//     });

//     for i in 1..5 {
//         println!("hi number {} from the main thread!", i);
//         thread::sleep(Duration::from_millis(1));
//     }
// }

fn main() -> Result<(), slint::PlatformError> {
    let ui = AppWindow::new()?;
    
    ui.on_request_login({
        move |data: User| {
            let username:slint::SharedString = data.username;
            //should send username and hashed password to api at {API-CON-STRING}
            //after awaiting and recieving a response it should either log in and open up the dashboard
            //or an error message should pop up asking the user to try again or create a new account
        }
    });
    ui.on_request_create_user({
        move |data: User| {
            //needs to use slint::spawn_local()
            //need to figure out how to use said functionality
            slint::spawn_local( async move {
                    let tokio_runtime = tokio::runtime::Runtime::new().unwrap();
                    let result = tokio_runtime.spawn(async move {
                        // This code is running on the Tokio runtime's thread, you can await futures that depends on Tokio here.
                        let mut map = HashMap::new();
                        map.insert("username", "data.username");
                        map.insert("password", "data.password");
                        let client = reqwest::Client::new();
                        let response = client.post("https://172.16.113.50:4321/createUser").json(&map).send().await;
                        print!("{response:?}")
                        
                    }).await.unwrap(); 
                    //assert_eq!(result, )
                }).unwrap();

                // thread::spawn(|| {
                //     let mut map = HashMap::new();
                //     map.insert("username", "data.username");
                //     map.insert("password", "data.password");
                //     let Client = reqwest::Client::new();
                //     let response = Client.post("http://172.16.113.50:4321/createUser")
                //     .json(&map)
                //     .send();
                //     print!("test1");
                // });
                
                // create_user(data);
                //It should take a username, and password, where it checks whether the first password is the same
                //should check if the data is proper
                //as the second after which it should send a post request to api and then "redirect" to login page
                //while displaying a confirmation of some sort.
                print!("test2");
                return(true);
            }
        });
        ui.run()
    }




    // async fn create_user(data:User) {
    //     // let passcode = env::var(constring)
    //     // let request_url: = env::var("DB_CON");
    //     let mut map = HashMap::new();
    //     map.insert("username", data.username);
    //     map.insert("password", data.password);
    //     let Client = reqwest::Client::new();
    //     let response = Client.post("http://172.16.113.50:4321/createUser")
    //     // .basic_auth(gh_user.clone(), Some(gh_pass.clone()))
    //     .json(&map)
    //     .send().await;
    //     // return match response.status()  {
    //     //     Err(e) => return Err(e),
    //     //     Some(t) => return Ok(t),
    //     // }
    // }