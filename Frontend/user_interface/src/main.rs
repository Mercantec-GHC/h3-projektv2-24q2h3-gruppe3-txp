// use std::env;
// use reqwest::Client;
slint::include_modules!();


fn main() -> Result<(), slint::PlatformError> {
    let ui = AppWindow::new()?;
 
        // ui.on_request_login({
        //     move |data: User| {
        //         let username:slint::SharedString = data.username;
        //         //should send username and hashed password to api at {API-CON-STRING}
        //         //after awaiting and recieving a response it should either log in and open up the dashboard
        //         //or an error message should pop up asking the user to try again or create a new account
        //     }
        // });
        // ui.on_request_create_user({
        //     move |data: User| {

        //         //It should take a username, and password, where it checks whether the first password is the same
        //         //should check if the data is proper
        //         //as the second after which it should send a post request to api and then "redirect" to login page
        //         //while displaying a confirmation of some sort.
                
        //     }
        // });
        ui.run()
    }




    // async fn create_user() {
    //     // let passcode = env::var(constring)
    //     let request_url = env::var("link");
    //     let response = Client::new()
    //     .post(request_url)
    //     // .basic_auth(gh_user.clone(), Some(gh_pass.clone()))
    //     .json("test")
    //     .send().await;
    //     // return match response.status()  {
    //     //     Err(e) => return Err(e),
    //     //     Some(t) => return Ok(t),
    //     // }
    // }