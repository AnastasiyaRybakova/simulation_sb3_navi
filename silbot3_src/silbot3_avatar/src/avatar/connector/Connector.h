#ifndef __CONNECTOR_H__
#define __CONNECTOR_H__

namespace avatar {
namespace connector{

class Connector {
public:
	virtual ~Connector(){}

	virtual void connect(const DataSource& datasource) = 0;
	virtual void close() = 0;
};
}}

#endif
