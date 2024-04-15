
slint::include_modules!();


fn main() -> Result<(), slint::PlatformError> {
    let ui = AppWindow::new()?;
 
        ui.on_request_login({
            move |data: User| {
                let username:slint::SharedString = data.username;
                println!("{username}")
            }
        });

    ui.run()
}