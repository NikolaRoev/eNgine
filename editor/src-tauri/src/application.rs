use tauri::Manager;


pub fn setup(app: &mut tauri::App) -> Result<(), Box<dyn std::error::Error>> {

    let binding = app.app_handle();
    let window = tauri::WindowBuilder::new(
        &binding,
        "main", tauri::WindowUrl::App("index.html".into()))
        .title("eNgine")
        //.menu(crate::menu::create_main_menu())
        .visible(false)
        .build().unwrap();

    Ok(())
}

pub fn callback(app: &tauri::AppHandle, event: tauri::RunEvent) {
    match event {
        tauri::RunEvent::Exit => log::info!("Exited zero."),
        _ => {}
    }
}
