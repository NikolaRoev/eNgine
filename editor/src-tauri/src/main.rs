#![cfg_attr(not(debug_assertions), windows_subsystem = "windows")]

mod api;
mod application;

fn main() {
    tauri::Builder::default()
        .plugin(tauri_plugin_log::Builder::default().targets([
            tauri_plugin_log::LogTarget::Stdout,
            tauri_plugin_log::LogTarget::LogDir
        ]).format(|format, args, record| {
            let (start, end) = match record.level() {
                log::Level::Error => ("\x1b[91m", "\x1b[0m"),
                log::Level::Warn => ("\x1b[93m", "\x1b[0m"),
                log::Level::Info => ("\x1b[92m", "\x1b[0m"),
                log::Level::Debug => ("\x1b[95m", "\x1b[0m"),
                log::Level::Trace => ("\x1b[94m", "\x1b[0m"),
            };

            format.finish(format_args!("{start}[{now}][{level}][{target}/{line}]: {args}{end}",
                start = start,
                now = chrono::Local::now().format("%F][%T%.9f"),
                level = record.level(),
                target = record.target(),
                line = record.line().unwrap_or(0),
                args = args,
                end = end
            ))
        }).build())
        .plugin(tauri_plugin_window_state::Builder::default().build())
        .plugin(tauri_plugin_single_instance::init(|app, _, _| {
            tauri::Manager::get_window(app, "main").map(|window| window.set_focus());
        }))
        .setup(application::setup)
        .invoke_handler(tauri::generate_handler![
            api::test
        ])
        //.on_menu_event(menu::event_handler)
        .build(tauri::generate_context!()).unwrap_or_else(|err| panic!("Failed to build application: {err}."))
        .run(application::callback);
}
