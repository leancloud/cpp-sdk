DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
find $DIR/../src -name "*.cpp" | xargs $DIR/../scripts/cpplint.py
find $DIR/../include -name "*.h" -not -path "$DIR/../include/json/*" | xargs $DIR/../scripts/cpplint.py
#find $DIR/../tests -name "*.h" -or -name "*.cpp" | xargs $DIR/../scripts/cpplint.py
