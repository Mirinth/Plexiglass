#include "RegularExpressionParser.hpp"

int main()
{
	parseRegularExpression("abc");
	parseRegularExpression("a|b|c");
	parseRegularExpression("a*");
	parseRegularExpression("abc|cba|(ab)*");

	parseRegularExpression("abc()");
	parseRegularExpression("abc)");
	parseRegularExpression("*abc");

	return 0;
}