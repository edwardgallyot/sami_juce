mod float;
mod bool_update;
mod int;
mod gesture;
pub use float::{
    set_float_update,
    get_float_update,
};
pub use int::{
    set_int_update,
    get_int_update,
};
pub use bool_update::{
    set_bool_update,
    get_bool_update,
};
pub use gesture::get_gesture_update;
