#ifndef GLOBAL_H
#define GLOBAL_H

#include <QString>
#include <QSqlQuery>

inline QSqlQuery getQuery(QString queryString){ //Applicable only to queries without passing any value except of queryString
   QSqlQuery query;
   query.setForwardOnly(true);
   query.prepare(queryString);
   query.exec();
   return query;
}

#endif // GLOBAL_H
