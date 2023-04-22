pub fn get_init_script() -> String {
    let init = include_str!("init.js");
    String::from(init)
}
