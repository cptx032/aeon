//		interpreter.cpp
//		
//		Copyright 2014 <cptx032@gmail.com>
//		
//		This program is free software; you can redistribute it and/or modify
//		it under the terms of the GNU General Public License as published by
//		the Free Software Foundation; either version 2 of the License, or
//		(at your option) any later version.
//      
//		This program is distributed in the hope that it will be useful,
//		but WITHOUT ANY WARRANTY; without even the implied warranty of
//		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//		GNU General Public License for more details.
//		
//		You should have received a copy of the GNU General Public License
//		along with this program; if not, write to the Free Software
//		Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
//		MA 02110-1301, USA.
//		
#include "aeon.cpp"

void show_intro()
{
	std::cout << "Aeon 1.0.0 (Apr 11 2014)" << std::endl;
	std::cout << "Type Ctrl+C to quit. \"help\" to see the var list." << std::endl;
}

void show_help()
{
	std::cout << "\n\n";
	AeonInterpreter *in = AeonInterpreter::get_default();
	std::cout << "------- VAR --------" << std::endl;
	
	for(uint i = 0; i < in->var_list.size();i++)
	{
		std::cout << i;
		if(i == in->var_index)
		{
			std::cout << " - ";
		}
		else
		{
			std::cout << " : ";
		}
		std::cout << in->var_list[i] << std::endl;
	}
	
	std::cout << "------- FUNC -------" << std::endl;
	for(uint i = 0; i < in->function_list.size(); i++)
	{
		std::cout << i;
		if(i == in->function_index)
		{
			std::cout << " - ";
		}
		else
		{
			std::cout << " : ";
		}
		std::cout << in->function_list[i].name << std::endl;
	}
	std::cout << "\n\n";
}

int main(int argc, char const *argv[])
{
	AeonInterpreter*i = new AeonInterpreter();
	
	AeonFunction pln("println", println);
	AeonFunction p("print", print);
	AeonFunction gi("input", get_input);
	AeonFunction r("run", run);
	AeonFunction re("read", read);
	
	i->function_list.push_back(pln);
	i->function_list.push_back(p);
	i->function_list.push_back(gi);
	i->function_list.push_back(r);
	i->function_list.push_back(re);
	
	if(argc >= 2)
	{
		std::string file_path = argv[1];
		std::vector<std::string> lines;
		std::ifstream script_file(file_path.c_str());
		std::string script_line;
		if(script_file.is_open())
		{
			while(getline(script_file, script_line))
			{
				lines.push_back(script_line);
			}
		}
		AeonError::AeonError error = i->run(lines);
		if (error == AeonError::SYNTAX_ERROR)
		{
			std::cout << "SyntaxError:" << std::endl;
		}
		else if(error == AeonError::CAST_ERROR)
		{
			std::cout << "CastError:" << std::endl;
		}
	}
	else{
	
	show_intro();
	while(true)
	{
		std::cout << ": ";
		std::string line;
		std::cin >> line;
		if(line == "help")
		{
			show_help();
			continue;
		}
		std::vector<std::string> v;
		v.push_back(line);
		AeonError::AeonError status = i->run(v);
		switch (status)
		{
			case AeonError::SYNTAX_ERROR:
			{
				std::cout << "SyntaxError ";
				std::cout << "line " << i->line_error;
				std::cout << " column: " << i->column_error << std::endl;
				break;
			}
			case AeonError::CAST_ERROR:
			{
				std::cout << "CastError ";
				std::cout << "line " << i->line_error;
				std::cout << " column: " << i->column_error << std::endl;
				break;
			}
			default:
			{
				break;
			}
		}
	}
}
	return 0;
}
// fixme: interpretador python
// fixme: ja inicializar 1 com argc e o resto com argv
