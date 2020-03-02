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
}
