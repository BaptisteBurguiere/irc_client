#include <Model.h>
#include <View.h> 
#include <Controller.h>

void displayHelp(void)
{
	std::cout << "better_client usage:" << std::endl;
	std::cout << "./better_client <server_ip> <server_port>" << std::endl;
}

int main(int argc, char **argv)
{
	if (argc != 3)
		displayHelp();
	else
	{
		View view;
		Model model;
		Controller controller(model, view);
		if (!controller.init(argv[1], argv[2]))
			std::cout << "Error: cannot connect to server " << argv[1] << " on port " << argv[2] << "." << std::endl;
		else
		{
			controller.mainLoop();
			controller.destroy();
		}
	}
	return 0;
}