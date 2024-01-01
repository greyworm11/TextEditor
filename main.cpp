#include "view.h"
#include "model.h"
#include "controller.h"

int main()
{
	View view;
	Model model(&view);
	Controller controller(&model);
	controller.start();

	return 0;
}