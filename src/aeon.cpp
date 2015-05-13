//		aeon.cpp
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
//		
#ifndef AEON_H
#define AEON_H
#include <string.h>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <sstream>
#include <fstream>

typedef void(*Function)();
typedef unsigned int uint;

std::string to_string(double value)
{
	std::ostringstream ss;
	ss << value;
	return ss.str();
}

namespace AeonError
{
	enum AeonError
	{
		NO_ERROR, CAST_ERROR, SYNTAX_ERROR
	};
}

class AeonFunction
{
public:
	std::string name;
	Function pointer;
	AeonFunction(std::string name, Function pointer)
	{
		this->name = name;
		this->pointer = pointer;
	}
	~AeonFunction(){}
	void call()
	{
		this->pointer();
	}
};

class AeonInterpreter
{
	private:
	static AeonInterpreter* default_interpreter;
	public:
	static AeonInterpreter *get_default()
	{
		return default_interpreter;
	}
	static void set_default(AeonInterpreter* d)
	{
		default_interpreter = d;
	}
	AeonInterpreter()
	{
		// inits the first value
		this->var_list.push_back("0");
		AeonInterpreter::set_default(this);
	}
	std::vector<AeonFunction> function_list;
	std::vector<std::string> var_list;
	uint line_error;
	uint column_error;
	uint function_index;
	uint var_index;
	AeonError::AeonError run(std::vector<std::string> script_lines)
	{
		bool get_error = false;
		AeonError::AeonError status = AeonError::NO_ERROR;
		for(uint line=0; line < script_lines.size();line++)
		{
			if (get_error)
			{
				// stop the loop
				break;
			}
			for(uint _c = 0; _c < strlen(script_lines[line].c_str()); _c++)
			{
				char t = script_lines[line][_c];
				if (t == '.')
				{
					this->function_list[this->function_index].call();
				}
				else if (t == '#'){break;}
				else if (t == '%')
				{
					this->var_list[this->var_index] = script_lines[line].substr(_c+1);
					break;
				}
				else if (t == '>')
				{
					this->var_index++;
					if (this->var_index == this->var_list.size())
					{
						this->var_list.push_back("0");
					}
				}
				else if (t == '<'){this->var_index--;}
				else if (t == 'v'){this->function_index++;}
				else if (t == '^'){this->function_index--;}
				else if (t == '+')
				{
					double d = atof(this->var_list[this->var_index].c_str());
					this->var_list[this->var_index] = to_string(d+1);
				}
				else if (t == '-')
				{
					double d = atof(this->var_list[this->var_index].c_str());
					this->var_list[this->var_index] = to_string(d-1);
				}
				else if (t == '(')
				{
					// while loop
					std::string while_block = script_lines[line].substr(_c+1);
					while(this->var_list[this->var_index] != "0")
					{
						std::vector<std::string> wb;
						wb.push_back(while_block);
						this->run(wb);
					}
					// jumpto the next line
					break;
				}
				else if (t == '{')
				{
					// the next char is '='?
					if (script_lines[line][_c+1] == '=')
					{
						if (this->var_list[this->var_index]==this->var_list[this->var_index-1])
						{
							// jump the next c
							_c++;
							continue;
						}
						else
						{
							// jump to next line
							break;
						}
					}
					// the next char is '|'?
					else if (script_lines[line][_c+1] == '|')
					{
						if (this->var_list[this->var_index] != this->var_list[this->var_index-1])
						{
							// jump the next char
							_c++;
							continue;
						}
						else
						{
							// jump to next line
							break;
						}
					}
					// the next char is ']'?
					else if (script_lines[line][_c+1] == ']')
					{
						std::istringstream ss_actual(this->var_list[this->var_index].c_str());
						double actual = 0;
						double last_value = 0;
						ss_actual >> actual;
						if(ss_actual.fail())
						{
							// casting error
							get_error  = true;
							status = AeonError::CAST_ERROR;
							this->line_error = line;
							this->column_error = _c;
							break;
						}
						std::istringstream ss_last(this->var_list[this->var_index-1].c_str());
						ss_last >> last_value;
						if(ss_last.fail())
						{
							// casting error
							get_error  = true;
							status = AeonError::CAST_ERROR;
							this->line_error = line;
							this->column_error = _c;
							break;
						}
						if (actual > last_value)
						{
							// jump the next char
							_c++;
							continue;
						}
						else
						{
							// jump to next line
							break;
						}
					}
					// the next char is '['?
					else if (script_lines[line][_c+1] == '[')
					{
						double actual = atof(this->var_list[this->var_index].c_str());
						double last_value = atof(this->var_list[this->var_index-1].c_str());
						if (actual < last_value)
						{
							// jump the next char
							_c++;
							continue;
						}
						else
						{
							// jump to next line
							break;
						}
					}
					// if is not any of these values
					else
					{
						get_error = true;
						status = AeonError::SYNTAX_ERROR;
						this->line_error = line;
						this->column_error = _c;
						break;
					}
				}
			}
		}
		return status;
	}
};
AeonInterpreter* AeonInterpreter::default_interpreter = NULL;
AeonInterpreter *i = new AeonInterpreter();
void print()
{
	std::cout << AeonInterpreter::get_default()->var_list[AeonInterpreter::get_default()->var_index];
}
void println()
{
	std::cout << AeonInterpreter::get_default()->var_list[AeonInterpreter::get_default()->var_index] << std::endl;
}
void get_input()
{
	std::cin >> AeonInterpreter::get_default()->var_list[AeonInterpreter::get_default()->var_index];
}
void run()
{
	AeonInterpreter *ae_i = AeonInterpreter::get_default();
	std::vector<std::string> line;
	std::string string_to_parse = ae_i->var_list[ae_i->var_index];
	// parsing string
	line.push_back("");
	for(uint i=0;i<strlen(string_to_parse.c_str());i++)
	{
		if(string_to_parse[i] == '\n')
		{
			line.push_back("");
		}
		else
		{
			line[line.size()-1] += string_to_parse[i];
		}
	}
	ae_i->run(line);
}
void read()
{
	AeonInterpreter *ae_i = AeonInterpreter::get_default();
	const char* file_path = ae_i->var_list[ae_i->var_index].c_str();
	std::ifstream script_file(file_path);
	std::string script_line;
	std::string final_string = "";
	if(script_file.is_open())
	{
		while(getline(script_file, script_line))
		{
			final_string += script_line;
			final_string += "\n";
		}
	}
	ae_i->var_list[ae_i->var_index-1] = final_string;
}
/*int main(int argc, char const *argv[])
{
	//AeonInterpreter::set_default(i);
	AeonFunction p("print", print);
	AeonFunction pl("println", println);
	AeonFunction gi("input", get_input);
	i->function_list.push_back(pl);
	i->function_list.push_back(p);
	i->function_list.push_back(gi);
	std::vector<std::string> lines;
	lines.push_back("%Digite algo: ");
	lines.push_back("v.v.^^.");
	i->run(lines);
	return 0;
}
*/
#endif
