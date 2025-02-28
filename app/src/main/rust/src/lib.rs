#[no_mangle]
pub extern "C" fn add_numbers(a: i32, b: i32) -> i32 {
  println!("✅ add_numbers() called in Rust with {} + {}", a, b);
  a + b
}
