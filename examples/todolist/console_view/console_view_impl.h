#pragma once

#include "console_view.h"
#include <vector>
#include <memory>
#include <map>

#include "tclap/CmdLine.h"

class ConsoleViewImpl
	:	public ConsoleView
{

public:

	ConsoleViewImpl(std::string const & _path_to_exec_file);

	void add_simple_command(ConsoleCommandType command_type, std::shared_ptr<TCLAP::SwitchArg> command_args) override final;
	void add_task_view_command(std::shared_ptr<TCLAP::SwitchArg> command_args) override final;
	void run_command_input() override final;
	void reset_task_view_commands();

	void activate_console(std::function<bool()> _callback) override final;

	bool is_command_set(ConsoleCommandType command_type) const override final;
	bool is_task_set(int & active_task_number) const override final;

private:

	std::unique_ptr<TCLAP::CmdLine> command_handler_;
	std::map<ConsoleCommandType, std::shared_ptr<TCLAP::SwitchArg> > commands_;
	std::vector<std::shared_ptr<TCLAP::SwitchArg>> task_commands_;

	const std::string path_to_exec_file_;
};