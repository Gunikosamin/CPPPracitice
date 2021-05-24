#pragma once
#include <memory>

//�}�`�f�[�^
#include "Object.h"

//�}�`�̕`��
class Shape
{
	//�}�`�f�[�^
	std::shared_ptr<const Object> object;

protected:

	//�`��Ɏg�����_�̐�
	const GLsizei vertexcount;

public:

	//�R���X�g���N�^
	// sieze:���_�̈ʒu�̎���
	// vertexcount: ���_�̐�
	// vertex: ���_�������i�[�����z��
	Shape(GLint size, GLsizei vertexcount, const Object::Vertex* vertex)
		: object(new Object(size, vertexcount, vertex))
		, vertexcount(vertexcount)
	{
	}
	//�`��
	void draw() const
	{
		// ���_�z��I�u�W�F�N�g����������
		object->bind();

		//�@�`������s����
		execute();
	}

	//�`��̎��sj
	virtual void execute() const
	{

		//�܂���ŕ`�悷��
		glDrawArrays(GL_LINE_LOOP, 0, vertexcount);

	}

};

