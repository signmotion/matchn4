// # В этот файл вынесены структуры, которые не дотягивают до класса,
//   но активно используются другими классами.


/**
* Вид игрового элемента.
*
* @example background, container
*/
typedef std::string  kindElement_t;




/**
* Название игрового элемента.
*/
typedef std::string  nameElement_t;




/**
* UID игрового элемента.
*/
typedef size_t  uidElement_t;




/**
* UID контейнера.
* @see Container
*/
typedef char  uidContainer_t;




/**
* Группа, название и UID события.
*/
typedef std::string  groupEvent_t;
typedef std::string  nameEvent_t;
typedef std::string  uidEvent_t;




/**
* Информация об источнике события.
*/
typedef std::string  sourceEvent_t;




/**
* Протокол обмена данными между событиями.
*/
typedef typelib::json::Variant  protocolDataEvent_t;
