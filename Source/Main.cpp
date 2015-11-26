// Main.cpp

#include "SVM.hpp" 
#include "Compiler/Compiler.hpp"
#include "ByteBuffer.hpp"
#include "Bytecode.hpp"
#include "Includes.hpp"
#include "Logger.hpp"
#include "OpcodeInfo.hpp"
#include "Types.hpp"
#include "Utils.hpp"
#include "StringUtils.hpp"
#include "Compiler/TU.hpp"
#include "Context.hpp"

void API_read(svm::Context &ctx)
{
	std::string in;
	std::cin >> in;
	ctx.stack.Push(new svm::Value(svm::SType::String, {.s_ = svm::StringUtils::ToString(in)}));
}

void API_print(svm::Context &ctx)
{
	svm::Value *v = ctx.stack.Pop();
	if (v == nullptr) return;

	switch (v->type)
	{
		case svm::SType::Number:
			std::cout << svm::Util::Type::ToStr(v->pool.n_);
			break;
		case svm::SType::String:
			std::cout << svm::StringUtils::ToCString(v->pool.s_);
			break;
		default: break;
	}

	delete v;
}

void API_strconcat(svm::Context &ctx)
{
	svm::Number n = ctx.stack.Top()->pool.n_;
	ctx.stack.Pop(true);

	std::stringstream ss;

	for (svm::Number i = 1; i < n; i++)
	{
		svm::Value *v = ctx.stack.Pop();
		if (v->type == svm::SType::String)
			ss << svm::StringUtils::ToCString(v->pool.s_);
		delete v;
	}

	ctx.stack.Push(new svm::Value(svm::SType::String, {.s_ = svm::StringUtils::ToString(ss.str())}));
	API_print(ctx);
}

int main(int argc, char **argv)
{
	//std::cout << "SimpleVM by ShrewdSpirit" << std::endl;

	if (argc != 3)
	{
		std::cout << "SimpleVM by ShrewdSpirit" << std::endl;
		std::cout << "Usage: -r/c/d/f/cr <file>\nFile names passed shouldnt have extentions" << std::endl;
		return 1;
	}

	enum class Mode { Run, Compile, Decompile, Fastrun, CompileRun } mode;

	if (strcmp(argv[1], "-r") == 0)
		mode = Mode::Run;
	else if (strcmp(argv[1], "-c") == 0)
		mode = Mode::Compile;
	else if (strcmp(argv[1], "-d") == 0)
		mode = Mode::Decompile;
	else if (strcmp(argv[1], "-f") == 0)
		mode = Mode::Fastrun;
	else if (strcmp(argv[1], "-cr") == 0)
		mode = Mode::CompileRun;
	else
	{
		std::cout << "Invalid switch" << std::endl;
		return 2;
	}

	if (!svm::Initialize())
	{
		std::cout << "Failed to initialize the VM" << std::endl;
		return 3;
	}

	switch (mode)
	{
		case Mode::Run:
			{
				try
				{
					std::string filename = argv[2]; filename += ".sc";
					svm::Context ctx;
					
					ctx.RegisterNative(API_print, "print");
					ctx.RegisterNative(API_read, "read");
					ctx.RegisterNative(API_strconcat, "strconcat");

					ctx.LoadFile(filename);
					ctx.Run();
				}
				catch (svm::BaseError &error) { std::cout << error; }
				catch (std::string &error) { std::cout << "[ERROR]" << error << std::endl; }
			}
			break;
		case Mode::Fastrun:
			{
				std::cout << "Not ready yet!" << std::endl;
				break;/*
				try
				{
					std::string filename = argv[2]; filename += ".s";
					std::string &program = svm::Util::File::ReadFile(filename);
					svm::Context ctx;
					
					ctx.RegisterNative(API_print, "print");
					ctx.RegisterNative(API_read, "read");
					ctx.RegisterNative(API_strconcat, "strconcat");

					ctx.LoadString(program);
					ctx.Run();
				}
				catch (svm::BaseError &error) { std::cout << error; }
				catch (std::string &error) { std::cout << "[ERROR]" << error << std::endl; }*/
			}
			break;
		case Mode::Compile:
			{
				std::string filename = argv[2]; filename += ".s";

				double compiletime = 0;
				svm::Util::HRT hrt;

				try
				{
					svm::Compiler::Compiler c(filename);
					svm::Bytecode &bc = c.DoCompile();
					compiletime = hrt.Elapsed();
					filename = argv[2]; filename +=  + ".sc";
					bc.WriteToFile(filename);
				}
				catch (svm::BaseError &error) { std::cout << error; }
				catch (std::string &error) { std::cout << "[ERROR]" << error << std::endl; }

				//std::cout << "Finished. Took " << hrt.Reset() << "s - " << compiletime << "s Compile time" << std::endl;
			}
			break;
		case Mode::CompileRun:
			{
				try
				{
					std::string filename = argv[2]; filename += ".s";
					
					svm::Compiler::Compiler c(filename);
					svm::Bytecode &bc = c.DoCompile();
					filename = argv[2]; filename +=  + ".sc";
					bc.WriteToFile(filename);

					filename = argv[2]; filename += ".sc";
					svm::Context ctx;
					
					ctx.RegisterNative(API_print, "print");
					ctx.RegisterNative(API_read, "read");
					ctx.RegisterNative(API_strconcat, "strconcat");
					ctx.LoadFile(filename);
					ctx.Run();
				}
				catch (svm::BaseError &error) { std::cout << error; }
				catch (std::string &error) { std::cout << "[ERROR]" << error << std::endl; }
			}
			break;
		case Mode::Decompile:
			{
				std::cout << "Not ready yet!" << std::endl;
				break;/*
				// Danger, don't use :p
				std::string filename = argv[2]; filename += ".sc";
				svm::Bytecode bc(filename);
				std::string &buffer = svm::Decompile(bc);
				filename = argv[2]; filename += ".dec.s";
				svm::Util::File::WriteFile(filename, buffer);*/
			}
			break;
	}
	
	return 0;
}
