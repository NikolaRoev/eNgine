use std::error::Error;
use web_sys::{WebGl2RenderingContext, WebGlProgram, WebGlShader};



pub struct Program {
    pub program: WebGlProgram
}

impl Program {
    fn shader(context: &WebGl2RenderingContext, shader_type: u32, source: &str) -> Result<WebGlShader, Box<dyn Error>> {
        let shader = context
            .create_shader(shader_type)
            .ok_or("Failed to create shader.")?;
        context.shader_source(&shader, source);
        context.compile_shader(&shader);

        let result = context
            .get_shader_parameter(&shader, WebGl2RenderingContext::COMPILE_STATUS)
            .as_bool()
            .unwrap_or(false);

        if result {
            Ok(shader)
        }
        else {
            Err(context
                .get_shader_info_log(&shader)
                .unwrap_or(String::from("Unknown error creating shader"))
                .into()
            )
        }
    }

    pub fn create(context: &WebGl2RenderingContext, vertex_source: &str, fragment_source: &str) -> Result<Self, Box<dyn Error>> {
        let vertex_shader = Program::shader(
            context,
            WebGl2RenderingContext::VERTEX_SHADER,
            vertex_source
        )?;

        let fragment_shader = Program::shader(
            context,
            WebGl2RenderingContext::FRAGMENT_SHADER,
            fragment_source
        )?;

        let program = context
            .create_program()
            .ok_or("Failed to create program")?;
        context.attach_shader(&program, &vertex_shader);
        context.attach_shader(&program, &fragment_shader);
        context.link_program(&program);


        let result = context
            .get_program_parameter(&program, WebGl2RenderingContext::LINK_STATUS)
            .as_bool()
            .unwrap_or(false);
        if result {
            Ok(Program{ program })
        }
        else {
            Err(context
                .get_program_info_log(&program)
                .unwrap_or(String::from("Unknown error creating program"))
                .into()
            )
        }
    }

    pub fn bind(&self, context: &WebGl2RenderingContext) {
        context.use_program(Some(&self.program));
    }
}
