module subject;
import <vector>;
import observer;

using namespace std;

void Subject::attach(Observer *ob) { observers.emplace_back(ob); }

void Subject::detach(Observer *ob) {
  for (auto it = observers.begin(); it != observers.end(); ++it) {
    if (*it == ob) {
      observers.erase(it);
      break;
    }
  }
}

void Subject::notifyObservers() const {
  for (auto ob : observers)
    ob->notify();
}

Subject::~Subject() {}
