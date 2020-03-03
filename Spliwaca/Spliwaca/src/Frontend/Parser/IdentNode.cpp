#include "Nodes.h"

namespace Spliwaca {
	std::string IdentNode::GetContents() {
		if (cachedContents != "")
			return cachedContents;
		std::string rv = "";
		//SPLW_INFO("{0}", ids.at(0)->GetContents());
		if (ids.at(0)->GetContents() != "_INTERPRETER") {
			rv += ids.at(0)->GetContents();
			for (size_t i = 1; i < ids.size(); i++) {
				rv += "." + ids.at(i)->GetContents();
			}
		} else if (ids.size() > 1) {
			rv += /*"__builtins__." +*/ ids.at(1)->GetContents();
			for (size_t i = 2; i < ids.size(); i++) {
				rv += "." + ids.at(i)->GetContents();
			}
		} else {
			SPLW_CRITICAL("Error: attempting to set something to/call _INTERPRETER. This is not allowed!");
		}
		cachedContents = rv;
		return rv;
	}

    std::string IdentNode::GenerateGetattrTree(bool &interpreter_var, bool minus_one) {
        if (cachedGetattrMinusOne != "" && minus_one)
            return cachedGetattrMinusOne;
        if (cachedGetattr != "")
            return cachedGetattr;

        if (ids.size() <= 1 && minus_one) {
            SPLW_CRITICAL("Attempting to get attribute of an identifier with no accesses. This is a bug.");
            return "";
        }
        if (ids.at(0)->GetContents() == "_INTERPRETER") {
            return GetContents();
        }
        std::string rv = "scope_vars[" + ids.at(0)->GetContents();

        for (int i = 1; i < ids.size() - 1; i++) {
            rv = "getattr(" + rv + ", \"" + ids[i]->GetContents() + "\")";
        }

        cachedGetattrMinusOne = rv;
        cachedGetattr = "getattr(" + rv + ", \"" + ids.at(ids.size() - 1)->GetContents() + "\")";

        return minus_one ? cachedGetattrMinusOne : cachedGetattr;
    }

	std::string IdentNode::GenerateGetattrTree(bool minus_one) {
        bool dummy_var = false;
        return GenerateGetattrTree(dummy_var, false);
    }

	std::string IdentNode::GetFinalId() {
        return ids.at(ids.size() - 1)->GetContents();
    }
}