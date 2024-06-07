#![windows_subsystem = "windows"]
fn main() {
    slint_build::compile("ui/mainwindow.slint").unwrap();
}