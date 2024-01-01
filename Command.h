class ICommand {
public:
	virtual void Execute() = 0;
	virtual void Undo() = 0;
	virtual bool get() = 0;
};


class ShowLine : public ICommand {
public:
	bool state = false;
	ShowLine() {
		this->state = false;
	}

	void Execute() override {
		this->state = true;
	}

	void Undo() override {
		this->state = false;
	}
	bool get() override {
		return this->state;
	}
};

class Command {
public:
	ICommand* comd = NULL;
	Command() {

	}

	Command(ICommand* cmd) {
		this->comd = cmd;
	}

	void onCommand() {
		this->comd->Execute();
	}

	void offCommand() {
		this->comd->Undo();
	}
};

