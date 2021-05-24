#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Window.h"
#include "Shape.h"




//�V�F�[�_�I�u�W�F�N�g�̃R���p�C�����ʂ�\������
// shader: �V�F�[�_�I�u�W�F�N�g��
// str: �R���p�C���G���[�����������ꏊ������������
GLboolean printShaderInfoLog(GLuint shader, const char* str)
{
	//�@�R���p�C�����ʂ��擾����
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE) std::cerr << "Compile Error in " << str << std::endl;

	// �V�F�[�_�̃R���p�C�����̃��O�̒������擾����
	GLsizei bufSize;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &bufSize);
	
	if (bufSize > 1)
	{
		//�V�F�[�_�̃R���p�C�����̃��O�̓��e���擾����
		std::vector<GLchar> infoLog(bufSize);
		GLsizei length;
		glGetShaderInfoLog(shader, bufSize, &length, &infoLog[0]);
		std::cerr << &infoLog[0] << std::endl;


	}
    
	return static_cast<GLboolean>(status); // �G���[�������true,�Ȃ����false;

}


//�@�v���O�����I�u�W�F�N�g���쐬����
// vsrc: �o�[�e�b�N�X�V�F�[�_�̃\�[�X�v���O�����̕�����
// fsrc: �t���O�����g�V�F�[�_�̃\�[�X�v���O�����̕�����
GLuint createProgram(const char *vsrc, const char *fsrc)
{
	//��̃v���O�����I�u�W�F�N�g���쐬����
	const GLuint program(glCreateProgram());


	if (vsrc != NULL)
	{

		 // �v���O�����I�u�W�F�N�g���쐬����
		 // vertex
		// �o�[�e�b�N�X�V�F�[�_�̃V�F�[�_�I�u�W�F�N�g���쐬����
		const GLuint vobj(glCreateShader(GL_VERTEX_SHADER));
		glShaderSource(vobj, 1, &vsrc, NULL);
		glCompileShader(vobj);

		//�@�o�[�e�b�N�X�V�F�[�_�̃V�F�[�_�I�u�W�F�N�g���v���O�����I�u�W�F�N�g�ɑg�ݍ���
		if (printShaderInfoLog(vobj , "vertex shader"))
		  glAttachShader(program, vobj);
	    glDeleteShader(vobj);


	}

	if (fsrc != NULL)
	{
		//�@�t���O�����g�V�F�[�_�̃V�F�[�_���I�u�W�F�N�g���v���O�����ɑg�ݍ���
		const GLuint fobj(glCreateShader(GL_FRAGMENT_SHADER));
		glShaderSource(fobj, 1, &fsrc, NULL);
		glCompileShader(fobj);

		// �t���O�����g�V�F�[�_�̃V�F�[�_�I�u�W�F�N�g���v���O�����I�u�W�F�N�g�ɑg�ݍ���
		if (printShaderInfoLog(fobj, "vertex shader"))
		  glAttachShader(program, fobj);
		  glDeleteShader(fobj);
	}

	//�@�v���O�����I�u�W�F�N�g�������N����
	glBindAttribLocation(program, 0, "position"); //�t�@�C���̉��ڂ�����o�����B
	glBindFragDataLocation(program, 0, "fragment"); //
	glLinkProgram(program);

   //�@�쐬�����v���O�����I�u�W�F�N�g��Ԃ�
	//if (printProgramInfoLog(program))
	return program;

	glDeleteProgram(program);
	return 0;
}

//�V�F�[�_�̃\�[�X�t�@�C����ǂݍ��񂾃�������Ԃ�
// name:�@�V�F�[�_�̃\�[�X�t�@�C����
// buffer: �ǂݍ��񂾃\�[�X�t�@�C���̃e�L�X�g
bool readShaderSource(const char* name, std::vector<GLchar> &buffer) {
   // �t�@�C������NULL������
	if (name == NULL) return false;

	// �\�[�X�t�@�C�����J��
	std::ifstream file(name, std::ios::binary);
	if (file.fail())
	{
		//�J���Ȃ�����
		std::cerr << "Error: Can't open source file: " << name << std::endl;
		return false;
	}

	// �t�@�C���̖����Ɉړ������݈ʒu(=�t�@�C���T�C�Y)�𓾂�
	file.seekg(0L, std::ios::end);
	GLsizei length = static_cast<GLsizei>(file.tellg());

	// �t�@�C���T�C�Y�̃�����
	buffer.resize(length + 1);

	//�@�t�@�C����擪����ǂݍ���
	file.seekg(0L, std::ios::beg);
	file.read(buffer.data(), length);
	buffer[length] = '\0';
	
	if (file.fail())
	{
		//���܂��ǂݍ��߂Ȃ�����
		std::cerr << "Error: Could not read source file: " << name << std::endl;
		file.close();
		return false;
	}

	//�ǂݍ��ݐ���
	file.close();
	return true;
}

//�V�F�[�_�̃\�[�X�t�@�C����ǂݍ���ŁA�v���O�����I�u�W�F�N�g���쐬����
// vert: �o�[�e�b�N�X�V�F�[�_�̃\�[�X�t�@�C����
// frag �t���O�����g�V�F�[�_�̃\�[�X�t�@�C�A��
GLuint loadProgram(const char *vert, const char *frag)
{
std::vector<GLchar> vsrc;
const bool vstat(readShaderSource(vert, vsrc));
std::vector<GLchar> fsrc;
const bool fstat(readShaderSource(frag, fsrc));

 //�v���O�����I�u�W�F�N�g���쐬����
return vstat && fstat ? createProgram(vsrc.data(), fsrc.data()) : 0;
}

// ��`�̒��_�̈ʒu
constexpr Object::Vertex rectangleVertex[] =
{
	{ -0.5f, -0.5f} ,
	{  0.5f, -0.5f} ,
	{  0.5f , 0.5f} ,
	{ -0.5f , 0.5f}

};

int main()
{
  // GLEW������������
	if (glfwInit() == GL_FALSE)
	{
      // �������Ɏ��s����
		std::cerr << "Can't initialize GLFW" << std::endl;
		return 1;
	}

	//�@�v���O�����I�����̏�����o�^����
	atexit(glfwTerminate);

	//OpenGL version 3.2 Core Profile��I������
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// �E�B���h�E���쐬����
	Window window;

	//�@�w�i�F�����Ă���
		glClearColor(0.0f, 0.0f, 0.0f, 0.5f);

	


		// �o�[�e�b�N�X�V�F�[�_�̃\�[�X�v���O����
	

		//�@�v���O�����I�u�W�F�N�g���쐬����
		const GLuint program(loadProgram("point.vert", "point.frag"));

		// uniform�@�ϐ��̏ꏊ���擾����
		const GLint sizeLoc(glGetUniformLocation(program, "size")); //uniform������v���O�����I�u�W�F�N�g��k, uniform�ϐ����̕�����
		const GLint scaleLoc(glGetUniformLocation(program, "scale"));
		const GLint locationLoc(glGetUniformLocation(program, "location"));

		// �}�`�f�[�^���쐬
		std::unique_ptr<const Shape> shape(new Shape(2, 4, rectangleVertex));

    // �E�B���h�E���J���Ă���ԂɌJ��Ԃ����C�����[�v
		while (window) {
		   
			//�E�B���h�E����������
			glClear(GL_COLOR_BUFFER_BIT);

			//�V�F�[�_�v���O�����̎g�p�J�n
			glUseProgram(program);

			// uniform�@�ϐ��ɒl��ݒ肷��
			glUniform2fv(sizeLoc, 1, window.getSize());
			glUniform1f(scaleLoc, window.getScale());
			glUniform2fv(locationLoc, 1, window.getLocation());

			//�}�`��`�悷��
			shape->draw();

			///
			///�@�����ŕ`�揈��
			///

			

			//�@�J���[�o�b�t�@�����ւ���
			window.swapBuffers();
			

		}

	
}