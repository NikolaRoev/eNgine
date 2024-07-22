pub mod scene_manager;
pub mod program;
pub mod renderer;

use std::error::Error;

use web_sys::{WebGl2RenderingContext, WebGlProgram, WebGlShader, console, WebGlUniformLocation};
use program::Program;
