#include "wtree2_viewer.hpp"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

const wtree<2>* wtree2_viewer::tree_ptr;

static void draw_dots(const wtree<2>& t, float depth)
{
	glColor3f(1.0*depth, 1.0*depth, 1.0*depth);
	glVertex2f(t.element[0], t.element[1]);
	
	for(size_t i = 0; i < wtree<2>::node_count; i++)
		if(t.nodes[i].count > 0)
			draw_dots(*t.nodes[i].wtree, depth*0.8);
}

static void draw_bounds(const wtree<2>& t, std::array<std::array<float, 2>, 2> bounds, float depth)
{
	for(size_t i = 0; i < wtree<2>::node_count; i++)
		if(t.nodes[i].count > 0)
		{
			std::array<std::array<float, 2>, 2> tmp_bounds = bounds;
			wtree<2>& new_t = *t.nodes[i].wtree;
		
			for(size_t d = 0; d < wtree<2>::dimension; d++)
				if(new_t.element[d] >= t.element[d])
					tmp_bounds[d][0] = t.element[d];
				else
					tmp_bounds[d][1] = t.element[d];
		
			draw_bounds(new_t, tmp_bounds, depth*0.5);
		}
	
	glColor3f(0.9*depth, 0.9*depth, 0.9*depth);

	for(size_t i = 0; i < wtree<2>::dimension; i++)
	{
		std::array<std::array<float, 2>, 2> line;
		
		for(size_t j = 0; j < 2; j++)
			line[j] = t.element;
		
		for(size_t j = 0; j < 2; j++)
			line[j][i] = bounds[i][j];
		
		for(size_t j = 0; j < 2; j++)
			glVertex2f(line[j][0], line[j][1]);
	}
}

void wtree2_viewer::draw()
{
	const wtree<2>& t = *tree_ptr;
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT);
	glTranslatef(-1.0, -1.0, 0.0);
	glScalef(2.0, 2.0, 1.0);
	
	glLineWidth(1.0);
	glBegin(GL_LINES);
	draw_bounds(t, {{{{0.0, 1.0}}, {{0.0, 1.0}}}}, 1.0);
	glEnd();
	
	glPointSize(5.0);
	glBegin(GL_POINTS);
	draw_dots(t, 1.0);
	glEnd();
	
	glFlush();
	glutSwapBuffers();
	glutPostRedisplay();
}

void wtree2_viewer::run(int argc, char** argv, const wtree<2>& tree)
{
	tree_ptr = &tree;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

	glutInitWindowPosition(100, 100);
	glutInitWindowSize(1024, 768);
	glutCreateWindow("wtree2");
	
	glutDisplayFunc(draw);
	
	glutMainLoop();
}
