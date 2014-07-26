/*
 * Audio_View.cpp
 *
 *  Created on: Dec 12, 2013
 *      Author: stens
 */

// Include GLM
#include <glm/glm.hpp>

// Include GLEW
#include <GL/glew.h>

//#include "../common/shader.hpp"
#include "../common/texture.hpp"
#include "../common/controls.hpp"

#include <GL/glfw.h> // IMPORTANT - put this AFTER others like  #include <glm/glm.hpp>  AND  #include <GL/glew.h>

#include "Audio_View.h"

#include <vector>
#include <iostream>
#include <sstream>		// ostringstream str

// --------------

GLuint scott_program;

struct struct_program {

    static GLuint Load( const char * vert, const char * geom, const char * frag ) {

        GLuint prog = glCreateProgram();
        if( vert ) AttachShader( prog, GL_VERTEX_SHADER, vert );
        if( geom ) AttachShader( prog, GL_GEOMETRY_SHADER, geom );
        if( frag ) AttachShader( prog, GL_FRAGMENT_SHADER, frag );
        glLinkProgram( prog );
        CheckStatus( prog );
        return prog;
    }

private:

    static void CheckStatus( GLuint obj ) {

        GLint status = GL_FALSE, len = 10;
        if( glIsShader(obj) )   glGetShaderiv ( obj, GL_COMPILE_STATUS, & status );
        if( glIsProgram(obj) )  glGetProgramiv( obj, GL_LINK_STATUS, & status );
        if( status == GL_TRUE ) return;
        if( glIsShader(obj) )   glGetShaderiv ( obj, GL_INFO_LOG_LENGTH, & len );
        if( glIsProgram(obj) )  glGetProgramiv( obj, GL_INFO_LOG_LENGTH, & len );
        std::vector< char > log( len, 'X' );
        if( glIsShader(obj) )   glGetShaderInfoLog ( obj, len, NULL, &log[0] );
        if( glIsProgram(obj) )  glGetProgramInfoLog( obj, len, NULL, &log[0] );
        std::cerr << & log[0] << std::endl;
        exit( -1 );
    }

    static void AttachShader( GLuint program, GLenum type, const char * src ) {

        GLuint shader = glCreateShader( type );
        glShaderSource( shader, 1, & src, NULL );
        glCompileShader( shader );
        CheckStatus( shader );
        glAttachShader( program, shader );
        glDeleteShader( shader );
    }
};

// ---

#define GLSL(version, shader) "#version " #version "\n" #shader

const char * vertex_shader = GLSL (
//	    400 core,
	    330 core,

    // Input vertex data, different for all executions of this shader.
    layout(location = 0) in vec3 vertexPosition_modelspace;
	layout(location = 1) in vec4 vcolor;

    out vec4 fcolor;

    // Values that stay constant for the whole mesh.
    uniform mat4 matrix_model_view_projection;

    void main() {

    	fcolor = vcolor;

    	// Output position of the vertex, in clip space : matrix_model_view_projection * position
    	gl_Position =  matrix_model_view_projection * vec4(vertexPosition_modelspace,1);

    	// UV of the vertex. No special space for this one.
//    	UV = vertexUV;
    }
);


// ----------------------------------------------


const char * fragment_shader = GLSL (

//	    400 core,
	    330 core,

    in vec4 fcolor;
	layout(location = 0) out vec4 FragColor;

	uniform float uniform_scott_flag_color;

    uniform float uniform_scott_Color_R;
	uniform float uniform_scott_Color_G;
	uniform float uniform_scott_Color_B;
	uniform float uniform_scott_Color_O;

//    out vec4 fColor;

    void main() {

    	if (0.0 == uniform_scott_flag_color) {

        	FragColor = fcolor;

    	} else {

//        fColor = vec4( 0.0, 0.85, 0.2, 0.1 );
//    	fColor = uniform_scott_Color;
//    	fColor = vec4( 0.0, uniform_scott_Color_G, 0.2, 0.1 );

    		FragColor = vec4(
    			uniform_scott_Color_R,
    			uniform_scott_Color_G,
    			uniform_scott_Color_B,
    			uniform_scott_Color_O);
    	}
    }
);


// -----------------------------------------------


void GLFWCALL resizeGLScene(int width, int height) {

	if (height == 0)    {                          // Prevent A Divide By Zero By

		height = 1;                                   // Making Height Equal One
	}

	std::cout << "resizeGLScene  "
			<< " width  "  << width
			<< " height " << height
			<< std::endl;

//	CurrentWidth = width;
//	CurrentHeight = height;

	glViewport(0, 0, width, height);               // Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);                 // Select The Projection Matrix
	glLoadIdentity();                             // Reset The Projection Matrix

	// calculate window aspect ratio

	gluPerspective(45.0f, (float) width / (float) height, 0.1f, 100.0f);

	glMatrixMode(GL_MODELVIEW);                   // Select The Modelview Matrix
	glLoadIdentity();                              // Reset The Modelview Matrix
}



void Audio_View::set_point_size(float given_point_size) {

	std::cout << "\nin View set_point_size    given_point_size " << given_point_size << std::endl;

	size_point_controllable = given_point_size;

	fprintf(stdout, "Audio_View  size_point_controllable %f\n", size_point_controllable);
}



void Audio_View::entry_point() {

	// ------------------

	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "ERROR - failed to initialize GLFW\n" );
		exit(8);
	}

	glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 3);
	glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	if (do_fullscreen) {

		// get the current Desktop screen resolution and colour depth
		GLFWvidmode desktop;
		glfwGetDesktopMode( &desktop );

		// open the window at the current Desktop resolution and colour depth
		if ( !glfwOpenWindow(
			desktop.Width,
			desktop.Height,
			desktop.RedBits,
			desktop.GreenBits,
			desktop.BlueBits,
			8,          // alpha bits
			32,         // depth bits
			0,          // stencil bits
			GLFW_FULLSCREEN
		) ) {
			// failed to open window: handle it here
		}

		std::cout << "going fullscreen" << std::endl;

	} else {

		// Open a window and create its OpenGL context
//		if( !glfwOpenWindow( 1024, 768, 0,0,0,0, 32,0, GLFW_WINDOW ) )
		if( !glfwOpenWindow( CurrentWidth, CurrentHeight, 0,0,0,0, 32,0, GLFW_WINDOW ) )
//		if( !glfwOpenWindow( desktop.Width / 3, desktop.Height / 3, 0,0,0,0, 32,0, GLFW_WINDOW ) )
		{
			fprintf( stderr, "ERROR - failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
			glfwTerminate();
			exit(8);
		}
	}

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "ERROR - failed to initialize GLEW\n");
		exit(8);
	}

//	texture_id = loadBMP_custom("/home/stens/Pictures/water_bear.bmp");

	 glfwSetWindowSizeCallback(resizeGLScene);

	// Ensure we can capture the escape key being pressed below
	glfwEnable( GLFW_STICKY_KEYS );
	glfwSetMousePos(1024/2, 768/2);

	if (flag_do_entire_universe_or_just_paddle) {

		// default background is black evidently -

	} else {

		// Dark blue background
		glClearColor(0.3f, 0.3f, 0.4f, 1.0f);

	//	glClearColor(1.0f, 1.0f, 1.0f, 01.0f);
	}

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	// Cull triangles which normal is not towards the camera
//	glEnable(GL_CULL_FACE);

	// --------  setup OpenGL  ----------------

	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
//	glPointSize(size_point);
//	glPointSize(size_point_controllable);

	// --------------

	int curr_num_object = 0;

	static const int vertex_array_object_index_paddle_indexed        = curr_num_object++;
	static const int vertex_array_object_index_audio_buffer_indexed  = curr_num_object++;

    GLuint vertex_array_object_handles[curr_num_object];         // one for each drawn object

    glGenVertexArrays(curr_num_object, &vertex_array_object_handles[0]);

    // -----------------  paddle neighbors - in pairs as lines indices air_neighbor_indices into air_location

    glBindVertexArray(vertex_array_object_handles[vertex_array_object_index_paddle_indexed]);	// paddle neighbor VAO

    // paddle vertex

    GLuint vertex_buffer_object_paddle_vertices;
    glGenBuffers(1, & vertex_buffer_object_paddle_vertices);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object_paddle_vertices);
	glBufferData(GL_ARRAY_BUFFER, audio_model->get_sizeof_paddle_location(), audio_model->get_address_paddle_location(), GL_DYNAMIC_DRAW);

    // set up generic attrib pointer

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    // paddle index

    // generate and bind the index buffer object
    GLuint vertex_buffer_object_paddle_neighbor_indices;
    glGenBuffers(1, & vertex_buffer_object_paddle_neighbor_indices);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertex_buffer_object_paddle_neighbor_indices);

    // fill with data
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, audio_model->get_sizeof_paddle_neighbor_indices(), audio_model->get_address_paddle_neighbor_indices(), GL_STATIC_DRAW);

    // -----------------  source audio curve -------------------------- //

    glBindVertexArray(vertex_array_object_handles[vertex_array_object_index_audio_buffer_indexed]);	// paddle neighbor VAO

    // paddle vertex

    GLuint vertex_buffer_object_audio_buffer_vertices;
    glGenBuffers(1, & vertex_buffer_object_audio_buffer_vertices);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object_audio_buffer_vertices);
	glBufferData(GL_ARRAY_BUFFER, audio_model->get_sizeof_audio_buffer(), audio_model->get_address_audio_buffer(), GL_DYNAMIC_DRAW);

    // set up generic attrib pointer

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    // paddle index

    // generate and bind the index buffer object
    GLuint vertex_buffer_object_audio_buffer_indices;
    glGenBuffers(1, & vertex_buffer_object_audio_buffer_indices);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertex_buffer_object_audio_buffer_indices);

    // fill with data
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, audio_model->get_sizeof_audio_buffer_indices(), audio_model->get_address_audio_buffer_indices(), GL_STATIC_DRAW);


    // --------  end of vertex objects ----------- //
    // --------  end of vertex objects ----------- //
    // --------  end of vertex objects ----------- //

	// Create and compile our GLSL program from the shaders
	scott_program = struct_program::Load(vertex_shader, NULL, fragment_shader);

	// Get a handle for our "matrix_model_view_projection" uniform
	GLuint MatrixID = glGetUniformLocation(scott_program, "matrix_model_view_projection");

	// Get a handle for our "myTextureSampler" uniform
	GLuint TextureID  = glGetUniformLocation(scott_program, "myTextureSampler");

	// ---------

	int count_num_molecular_update_chronos_view = 0;

    // Initialize time
    double time = glfwGetTime(), delta_time = 0;            // Current time and elapsed time
    double frameDTime = 0, frameCount = 0, fps = 0; // Framerate
//    double cumulative_fps = 0;
    double cumulative_frameDTime = 0, cumulative_frameCount = 0;

	do {

		glPointSize(size_point_controllable);

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black and opaque
//	    glClearColor(0.0f, 0.0f, 0.20f, 1.0f); // dark blue and opaque

        // Get elapsed time
	    delta_time = glfwGetTime() - time;
        if (delta_time < 0) delta_time = 0;
        time += delta_time;

		// Use our shader
		glUseProgram(scott_program);

		audio_model->update_location_molecules(count_num_molecular_update_chronos_view);

		// Compute the matrix_model_view_projection matrix from keyboard and mouse input
		computeMatricesFromInputs();
		glm::mat4 ProjectionMatrix = getProjectionMatrix();
		glm::mat4 ViewMatrix = getViewMatrix();
		glm::mat4 ModelMatrix = glm::mat4(1.0);
		glm::mat4 matrix_model_view_projection = ProjectionMatrix * ViewMatrix * ModelMatrix;

		// Send our transformation to the currently bound shader,
		// in the "matrix_model_view_projection" uniform
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, & matrix_model_view_projection[0][0]);
		glEnableVertexAttribArray(0);

		// ------------------------- start of each object to be visualized ------------------ //



		// --------------  paddle neighbor indexed

		// if below sets value of 1.0 then do uniform colors ELSE do 2nd parm given color
		GLint flag_uniform_or_not = glGetUniformLocation(scott_program, "uniform_scott_flag_color");
		if (flag_uniform_or_not != -1) {
		glUniform1f(flag_uniform_or_not, 0.0);	// NOTE it sets value 0.0 to flag use 2nd parm
		}

	    glBindVertexArray(vertex_array_object_handles[vertex_array_object_index_paddle_indexed]);
	    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object_paddle_vertices);
		glBufferData(GL_ARRAY_BUFFER, audio_model->get_sizeof_paddle_location(), audio_model->get_address_paddle_location(), GL_DYNAMIC_DRAW);

	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertex_buffer_object_paddle_neighbor_indices);
	    glBufferData(GL_ELEMENT_ARRAY_BUFFER, audio_model->get_sizeof_paddle_neighbor_indices(), audio_model->get_address_paddle_neighbor_indices(), GL_STATIC_DRAW);

        glDrawElements(GL_LINES, audio_model->get_curr_paddle_neighbor_index(), GL_UNSIGNED_INT, 0);

		// --------------  audio buffer indexed

		// if below sets value of 1.0 then do uniform colors ELSE do 2nd parm given color
		flag_uniform_or_not = glGetUniformLocation(scott_program, "uniform_scott_flag_color");
		if (flag_uniform_or_not != -1) {
		glUniform1f(flag_uniform_or_not, 0.0);	// NOTE it sets value 0.0 to flag use 2nd parm
		}

	    glBindVertexArray(vertex_array_object_handles[vertex_array_object_index_audio_buffer_indexed]);
	    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object_audio_buffer_vertices);
		glBufferData(GL_ARRAY_BUFFER, audio_model->get_sizeof_audio_buffer(), audio_model->get_address_audio_buffer(), GL_DYNAMIC_DRAW);

	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertex_buffer_object_audio_buffer_indices);
	    glBufferData(GL_ELEMENT_ARRAY_BUFFER, audio_model->get_sizeof_audio_buffer_indices(), audio_model->get_address_audio_buffer_indices(), GL_STATIC_DRAW);

        glDrawElements(GL_LINES, audio_model->get_curr_audio_buffer_indices_index(), GL_UNSIGNED_INT, 0);

	    // --------  end of display objects ----------- //
	    // --------  end of display objects ----------- //
	    // --------  end of display objects ----------- //

		// ----------------

		// deactivate vertex arrays after drawing
		glDisableClientState(GL_VERTEX_ARRAY);

		// ------------
		// Swap buffers
		glfwSwapBuffers();

		// --------


//		glfwSetWindowTitle( "cpp_aw_openal_camera_10" );

        // Estimate framerate
        frameCount++;
        frameDTime += delta_time;


        cumulative_frameCount++;
        cumulative_frameDTime += delta_time;


        if( frameDTime > 1.0 ) {
//        if( frameDTime > 0.5 ) {

            fps = frameCount/frameDTime;
            char newTitle[128];

//            cumulative_fps = cumulative_frameCount / cumulative_frameDTime;


    		sprintf(
    				newTitle,
//        			"%s fish %s sharks board %s doing %.1f Frames Per Second @ %d x %d  %s",
        			"%.1f Frames Per Second @ %d x %d  %s",
//    			insert_commas_into_int(audio_model->get_num_alive_fish()).c_str(),
//    			insert_commas_into_int(audio_model->get_num_alive_shark()).c_str(),
//    			insert_commas_into_int(audio_model->get_board_max()).c_str(),
    			fps,
    			CurrentWidth,
    			CurrentHeight,
    			WINDOW_TITLE_PREFIX.c_str()
    		);



//    		std::cout << " fps " << fps
//    				<< " cumulative " << cumulative_fps
//    				<< std::endl;


            frameCount = frameDTime = 0;

    		glfwSetWindowTitle( newTitle );

    		// -------------
        }

		// --------

		count_num_molecular_update_chronos_view++;

		continue_processing = audio_model->get_flag_continue_processing();

	} // check if the ESC key was pressed or the window was closed
	while( glfwGetKey( GLFW_KEY_ESC ) != GLFW_PRESS &&
		   glfwGetWindowParam( GLFW_OPENED ) && continue_processing );

	// Cleanup VBO and shader
	glDeleteProgram(scott_program);
	glDeleteTextures(1, &TextureID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	// -------
}

Audio_View::Audio_View(Audio_Model * given_audio_model) {	// constructor

	audio_model = given_audio_model;

	do_fullscreen = false;

	flag_do_entire_universe_or_just_paddle = false;

	continue_processing = true;

	size_point_controllable = 0;
	CurrentWidth = 0;
	CurrentHeight = 0;
}

Audio_View::~Audio_View() {}

