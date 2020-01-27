#include "Nodes.h"
#include "UtilFunctions.h"

namespace Spliwaca
{
	VarType AddExprNode::GetExprReturnType()
	{
		VarType leftType = left->GetExprReturnType();
		VarType rightType = right->GetExprReturnType();

		if (leftType == VarType::String && rightType == VarType::Float)
		{
			RegisterSemanticError({})
		}
	}
}