export module subject;

import <vector>;
import observer;

export class Subject {
  std::vector<Observer *> observers;

public:
  void attach(Observer *ob);
  void detach(Observer *ob);
  void notifyObservers() const;
  virtual ~Subject();
};
