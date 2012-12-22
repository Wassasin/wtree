#include "wtree3_viewer.hpp"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

void* tree_ptr;
static size_t i;

static void draw_dots(const wtree<3>& t, float depth)
{
	glColor3f(1.0*depth, 1.0*depth, 1.0*depth);
	glVertex3f(t.element[0], t.element[1], t.element[2]);
	
	for(size_t i = 0; i < wtree<3>::node_count; i++)
		if(t.nodes[i].count > 0)
			draw_dots(*t.nodes[i].wtree, depth*0.7);
}

static void draw_bounds(const wtree<3>& t, std::array<std::array<float, 2>, 3> bounds, float depth)
{
	for(size_t i = 0; i < wtree<3>::node_count; i++)
		if(t.nodes[i].count > 0)
		{
			std::array<std::array<float, 2>, 3> tmp_bounds = bounds;
			wtree<3>& new_t = *t.nodes[i].wtree;
		
			for(size_t d = 0; d < wtree<3>::dimension; d++)
				if(new_t.element[d] >= t.element[d])
					tmp_bounds[d][0] = t.element[d];
				else
					tmp_bounds[d][1] = t.element[d];
		
			draw_bounds(new_t, tmp_bounds, depth*0.5);
		}
	
	glColor3f(0.9*depth, 0.9*depth, 0.9*depth);

	for(size_t i = 0; i < wtree<3>::dimension; i++)
	{
		std::array<std::array<float, 3>, 2> line;
		
		for(size_t j = 0; j < 2; j++)
			line[j] = t.element;
		
		for(size_t j = 0; j < 2; j++)
			line[j][i] = bounds[i][j];
		
		for(size_t j = 0; j < 2; j++)
			glVertex3f(line[j][0], line[j][1], line[j][2]);
	}
}

static float in_range(size_t i, size_t steps, float from, float to)
{
	float range = to - from;
	float step = range / (float)steps;
	
	size_t j = i % steps;
	
	return from + step * ((float) j);
}

void wtree3_viewer::run(int argc, char** argv, const wtree<3>& tree)
{
	tree_ptr = (void*)&tree;
	i = 0;
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

	glutInitWindowPosition(100, 100);
	glutInitWindowSize(1024, 768);
	glutCreateWindow(" ");
	
	glutDisplayFunc(draw);
	
	glutMainLoop();
}

void wtree3_viewer::draw()
{
	const wtree<3>& t = *static_cast<wtree<3>*>(tree_ptr);
	
	constexpr float dist = 2.0;
	float rad = in_range(i, 360, 0, 2.0 * M_PI);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, 1.333, 1.0, 100.0); 
	gluLookAt(0.5+dist*std::cos(rad), 0.5, 0.5+dist*std::sin(rad), 0.5, 0.5, 0.5, 0.0, 1.0, 0.0);
	//glTranslatef(-0.5, -0.5, 0);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT);
	
	glLineWidth(5.0);
	glBegin(GL_LINES);
	for(size_t i = 0; i < 3; i++)
	{
		glColor3f(i == 0, i == 1, i == 2);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(i == 0, i == 1, i == 2);
	}
	glEnd();
	
	glLineWidth(1.0);
	glBegin(GL_LINES);
	draw_bounds(t, {{{{0.0, 1.0}}, {{0.0, 1.0}}, {{0.0, 1.0}}}}, 1.0);
	glEnd();
	
	glPointSize(3.0);
	glBegin(GL_POINTS);
	draw_dots(t, 1.0);
	glEnd();

	glFlush();
	glutSwapBuffers();
	glutPostRedisplay();
	
	i++;
}
