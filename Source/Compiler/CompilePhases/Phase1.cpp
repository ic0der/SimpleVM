// Phase1.cpp

#include "../Compiler.hpp" 
#include "../SyntaxHelper.hpp"

namespace svm
{
	namespace Compiler
	{
		void Compiler::Phase1()
		{
			std::string cleared_program;

            /* Get rid of shitty newline/whitespace characters */
            for (Length pi = 0; pi < m_program.length(); pi++)
            {
            	switch(m_program[pi])
            	{
        			case '\t':
        				cleared_program += ' ';
        				break;
            		case '\r':
            			break;
        			case '\v':
        			case '\f':
            			cleared_program += '\n';
            			break;
            		default:
            			cleared_program += m_program[pi];
            			break;
            	}
            }

            cleared_program += '\n';
            m_program = cleared_program;
            cleared_program = "";

            /* Clear comments */
            bool is_incomment = false,
            	is_instring = false;

            for (Length pi = 0; pi < m_program.length(); pi++)
            {
            	char cc = m_program[pi];
            	char pc = pi > 0 ? m_program[pi - 1] : '\0';

            	if ((cc == '\n' || SyntaxHelper::IsCommentChar(cc)) && is_incomment)
            	{
            		is_incomment = false;
            		if (cc == '\n')
            			cleared_program += cc;
            		else
            			cleared_program += ' ';
            		continue;
            	}
            	else if (SyntaxHelper::IsCommentChar(cc) && !is_instring)
            	{
            		is_incomment = true;
            		continue;
            	}

            	if (!is_incomment)
            		cleared_program += cc;

            	if (SyntaxHelper::IsStringLiteral(cc) && is_instring && pc != '\\')
            	{
            		is_instring = false;
            		continue;
            	}
            	else if (SyntaxHelper::IsStringLiteral(cc) && !is_incomment)
            		is_instring = true;
            }

            m_program = cleared_program;
		}
	}
}
