#![windows_subsystem = "windows"]

use dotenv;
use reqwest::{Client, Error, Response};
use serde::{Deserialize, Serialize};

use slint::{ModelRc, SharedString, VecModel};
use std::{collections::HashMap, env, rc::Rc, time::SystemTime};
slint::include_modules!();

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
struct Device {
    id: i32,
    #[serde(rename = "ArduinoDevice")]
    arduino_device: String,
    #[serde(rename = "Account")]
    account: Option<String>,
}
#[derive(Serialize, Deserialize, Debug)]
struct Username {
    username: String,
}
#[derive(Serialize, Deserialize, Debug,Clone)]
struct userscore {
    username: String,
    score: i32,
}


const API_CON: &str = "https://h3-projektv2-24q2h3-gruppe3-txp.onrender.com/";
fn main() -> Result<(), slint::PlatformError> {
    let ui = AppWindow::new().unwrap();
    let ui_weak1 = ui.as_weak();
    let ui_weak2 = ui.as_weak();
    let ui_weak3 = ui.as_weak();
    let ui_weak4 = ui.as_weak();
    
    let _ = dotenv::dotenv().is_ok();
    ui.on_getdevices({
        move || {
            let tokio_runtime = tokio::runtime::Runtime::new().unwrap();
            let handle = ui_weak2.unwrap();
            handle
            .global::<logic>()
            .set_error("Awaiting API".into());
            let _ = slint::spawn_local(async move {
                let res = tokio_runtime
                    .spawn(async move {
                        let client: Client = Client::new();
                        let res = client
                            .get("https://h3-projektv2-24q2h3-gruppe3-txp.onrender.com/getDevices")
                            .send()
                            .await
                            .expect("error. it failed")
                            .json::<Vec<Device>>()
                            .await;
                        return res;
                    })
                    .await;
                let temp = match res {
                    Ok(data) => data,
                    Err(error) => panic!("{error}"),
                };
                let result = match temp {
                    Ok(data) => data,
                    Err(error) => panic!("{error}"),
                    
                };
                let result: Rc<VecModel<SharedString>> = Rc::new(vec_device(result));
                handle
                    .global::<logic>()
                    .set_Devices(ModelRc::<SharedString>::new(result.clone()));
                handle
                .global::<logic>()
                .set_error("".into());
            });
        }
    });
    ui.on_login({
        move |creds: Credentials, device: SharedString| {
            let handle = ui_weak3.unwrap();
            let tokio_runtime = tokio::runtime::Runtime::new().unwrap();
           let _ = slint::spawn_local(async move {
                let res = tokio_runtime
                    .spawn(async move {
                        let con_string: &str = &format!("{}{}", API_CON, "login");
                        let mut map: HashMap<&str, String> = HashMap::new();
                        map.insert("username", creds.username.to_string());
                        map.insert("password", creds.password.to_string());
                        map.insert("arduinoDevice", device.to_string());
                        let client: Client = Client::new();
                        let response: Result<Response, Error> =
                            client.post(con_string).json(&map).send().await;
                        return response;
                    })
                    .await;
                let temp_binding = match res {
                    Ok(data) => data,
                    Err(error) => panic!("{error}"),
                };
                let binding = match temp_binding {
                    Ok(data) => data,
                    Err(error) => panic!("{error}"),
                };
                let test = binding.cookies().next();
                    match test {
                    Some(test) => {
                        let cookie: SharedString = test.value().into();
                        handle.clone_strong().global::<logic>().set_token(cookie);
                        handle.global::<logic>().set_logged_in(true);
                        handle.global::<logic>().set_active_page(0);
                        handle.global::<logic>().set_sidebaractive(true);
                        handle.global::<logic>().set_sidebartest(true);
                    }
                    None => {handle
                        .clone_strong()
                        .global::<logic>()
                        .set_error("Invalid login".into());
                    }
                };
            });

        }
    });
    ui.on_createacc({
        move |data: Credentials| {
            let tokio_runtime = tokio::runtime::Runtime::new().unwrap();
            slint::spawn_local(async move {
                let _res: Result<std::string::String, Error> = tokio_runtime
                    .spawn(async move {
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
                        return response;
                    })
                    .await
                    .unwrap();
            })
            .unwrap();
        }
    });
    ui.on_gethighscores({
        move |data: i32, token: SharedString| {
            let tokio_runtime = tokio::runtime::Runtime::new().unwrap();
            let handle = ui_weak1.unwrap();
            let _ = slint::spawn_local(async move {
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
                        return response;
                    })
                    .await
                    .unwrap();
                let result: Rc<VecModel<User>> = Rc::new(vec_test(res));
                handle
                    .global::<logic>()
                    .set_value(ModelRc::<User>::new(result.clone()));
                return result;
            });
        }
    });
    ui.on_getuserdate({move |token: SharedString| {
        print!("test");
        let tokio_runtime = tokio::runtime::Runtime::new().unwrap();
        let handle = ui_weak4.unwrap();
        let _ = slint::spawn_local(async move {
            let res  = tokio_runtime
                .spawn(async move {
                    let frk = format!("JWT={}", token).to_string();
                    print!("{frk:#?}");
                    let con_string: &str = &format!("{}{}", API_CON, "getUserStats");
                    let client: Client = Client::new();
                    let response = client
                        .post(con_string)
                        .header("cookie", frk)
                        .send()
                        .await
                        .expect("failed")
                        .json::<userscore>().await;
                        print!("{response:#?}");
                        let temp = match response {
                            Ok(data)=> data,
                            Err(error) => panic!("{error}"),
                        };
                    return temp;
                })
                .await;
                print!("{res:#?}");
                let data = match res {
                    Ok(data) => data,
                    Err(error)=> panic!("{error}"),
                };
            handle.global::<logic>().set_userscore(data.score.into())
                
    });
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
fn vec_device(api_output: Vec<Device>) -> VecModel<SharedString> {
    let output: VecModel<SharedString> = Vec::new().into();

    for score in api_output {
        let temp_val: SharedString = score.arduino_device.into();

        output.push(temp_val);
    }
    return output;
}

fn get_time() {
let now: SystemTime = SystemTime::now();

print!("{now:#?}");
}