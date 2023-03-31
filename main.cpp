#include "include/server.hpp"
#include "include/utils.hpp"
#include "include/errors.hpp"
#include "include/launch.hpp"

int main(int ac, char **av)
{
	// parse config file;
	Web servers;
	try
	{
		std::string __config = __config_file(ac, av);

		servers.__parse(__config);

		servers.__set_locations();

		Server_launch __launcher(&servers.__servers);

		__launcher.__launch();
		__launcher.__run();
		__servers_display(servers);
	}

	catch (const std::exception &e)
	{
		std::cout << e.what() << std::endl;
		// std::cerr << "\033[1;31m" << e.what() << "\033[0m" << std::endl;
		std::cerr << "\033[1;31m"
							<< "error: bad synatx"
							<< "\033[0m" << std::endl;
	}

	return 0;
}