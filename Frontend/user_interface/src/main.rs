use dotenv;
use reqwest::{Client, Error, Response};
use serde::{Deserialize, Serialize};
use slint::{ModelRc, SharedString, VecModel};
use std::{collections::HashMap, env, rc::Rc};

slint::include_modules!();

#[derive(Serialize, Deserialize)]
struct ServerResponse {
    message: String,
}

#[derive(Serialize, Deserialize)]
struct LoginResponse {
    token: String,
    date: String,
    status: i16,
}

#[derive(Serialize, Deserialize, Debug)]
struct Score {
    #[serde(rename = "gameId")]
    game_id: i32,
    id: i32,
    #[serde(rename = "replayId")]
    replay_id: i32,
    score: i32,
    #[serde(rename = "userId")]
    user_id: i32,
    #[serde(rename = "userRelation")]
    user_relation: Username,
}
#[derive(Serialize, Deserialize, Debug)]
struct Username {
    username: String,
}

const API_CON: &str = "https://h3-projektv2-24q2h3-gruppe3-txp.onrender.com/";
fn main() -> Result<(), slint::PlatformError> {
    let ui = AppWindow::new().unwrap();
    let ui_weak = ui.as_weak();
    let _ = dotenv::dotenv().is_ok();
    ui.on_login({
        move |data: Credentials| {
            println!("entered login handler");
            let tokio_runtime = tokio::runtime::Runtime::new().unwrap();

            let (sender, reciever) = std::sync::mpsc::channel::<SharedString>();

            let thread = std::thread::spawn(move || {
                println!("entered slint spawned");
                let res = tokio_runtime.block_on(async move {
                    println!("entered tokio spawned");
                    let con_string: &str = &format!("{}{}", API_CON, "login");
                    let mut map: HashMap<&str, String> = HashMap::new();
                    map.insert("username", data.username.to_string());
                    map.insert("password", data.password.to_string());
                    let client: Client = Client::new();

                    let response: Result<Response, Error> =
                        client.post(con_string).json(&map).send().await;
                    let data: Response = match response {
                        Ok(data) => data,
                        Err(error) => panic!("panicked {}", error),
                    };
                    let binding = data;
                    let mut test = binding.cookies();
                    let fest = test.next().unwrap();
                    let frick: SharedString = fest.value().into();
                    frick
                });

                sender.send(res).unwrap();
                println!("send message")
            });

            print!("waiting for message");
            let res = reciever.recv().unwrap();
            println!("{res:#?}");

            // let ui_clone: slint::Weak<AppWindow> = ui.clone();
            // ui_clone.global::<logic>().set_loading(true);
            thread.join().unwrap();
            return res;
        }
    });
    ui.on_createacc({
        move |data: Credentials| {
            //need to figure out how to use said functionality
            //creates a new user in the database
            let tokio_runtime = tokio::runtime::Runtime::new().unwrap();
            slint::spawn_local(async move {
                let res: Result<std::string::String, Error> = tokio_runtime
                    .spawn(async move {
                        // This code is running on the Tokio runtime's thread, you can await futures that depends on Tokio here.
                        let mut map: HashMap<&str, String> = HashMap::new();
                        map.insert("username", data.username.to_string());
                        map.insert("password", data.password.to_string());
                        let client: Client = Client::new();
                        let response = client
                            .post("https://h3-projektv2-24q2h3-gruppe3-txp.onrender.com/createUser")
                            .json(&map)
                            .send()
                            .await
                            .expect("failed to get response")
                            .text()
                            .await;
                        print!("{response:#?}");

                        return response;
                    })
                    .await
                    .unwrap();
                print!("{res:#?}");
                if res.is_ok() {}

                //let result = from_str::<server_response>(result);
            })
            .unwrap();
        }
    });
    ui.on_gethighscores({
        move |data: i32, token: SharedString| {
            let tokio_runtime = tokio::runtime::Runtime::new().unwrap();
            let handle =ui_weak.unwrap();
            let _ = slint::spawn_local(async move {

                // let handle = AppWindow::new().unwrap();
                let res: Vec<Score> = tokio_runtime
                    .spawn(async move {
                        let mut map: HashMap<&str, i32> = HashMap::new();
                        map.insert("gameId", data);
                        let frk = format!("JWT={}", token).to_string();
                        let con_string: &str = &format!("{}{}", API_CON, "getHighscores");
                        let client: Client = Client::new();
                        let response: Vec<Score> = client
                            .get(con_string)
                            .header("cookie", frk)
                            .json(&map)
                            .send()
                            .await
                            .expect("failed")
                            .json::<Vec<Score>>()
                            .await
                            .expect("failed at json conversion");
                        print!("{response:#?}");
                        return response;
                    })
                    .await
                    .unwrap();
                let result: Rc<VecModel<User>> = Rc::new(vec_test(res));
                handle
                    .global::<logic>()
                    .set_value(ModelRc::<User>::new(result.clone()));
                println!("{:#?}", ModelRc::<User>::new(result.clone()));
                println!("{:#?}", handle.global::<logic>().get_value());
                return result;
            })
            .unwrap();

            let test2: ModelRc<User> = Default::default();
            return test2;
        }
    });
    ui.run()
}

fn vec_test(api_output: Vec<Score>) -> VecModel<User> {
    let output: VecModel<User> = Vec::new().into();

    for score in api_output {
        let temp_val: User = User {
            name: score.user_relation.username.into(),
            score: score.score,
            identifier: score.id,
        };
        output.push(temp_val);
    }
    return output;
}
