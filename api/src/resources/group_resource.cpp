#include "group_resource.h"
#include "root_requirements.h"
#include "message_queue.h"

void GroupResource::deleteRequirements(const HttpRequest& request, Session& session, const Ptr<Group>& group) const {
    RootRequirements::requireTeacherRoots(request, session);
}

void GroupResource::processPatch(const HttpRequest& request, Session& session, const Ptr<Group>& group) const {
    RootRequirements::requireTeacherRoots(request, session);

    for (const auto& [key, value] : request.body()) {
        if (key == Str::name) {
            auto name = static_cast<std::string>(value);

            if (session.exist(&Session::getByName<Group>, name)) {
                throw UnprocessableEntityException("Name already exists");
            }

            group.modify()->setName(name);
        }
        else if (key == Str::cfGroupCode) {
            group.modify()->setCfGroupCode(static_cast<std::string>(value));
        }
    }
}

Ptr<Group> GroupResource::createObject(const Wt::Json::Object& json, Session& session) const {
    auto name = static_cast<std::string>(json.at(Str::name));

    if (session.exist(&Session::getByName<Group>, name)) {
        throw UnprocessableEntityException("Group already exists");
    }

    return session.add(std::make_unique<Group>(std::move(name)));
}

void GroupResource::sendUpdatedInfo(const Ptr<Group>& group) const {
    auto message = static_cast<Wt::Json::Object>(*group);
    message[Str::groupId] = group.id();
    MessageQueue::getInstance().publish("algo_data", message);
}

void GroupResource::sendDeletedInfo(const Ptr<Group>& group) const {
    Wt::Json::Object message;
    message[Str::groupId] = group.id();
    message["deleted"] = true;
    MessageQueue::getInstance().publish("algo_data", message);
}
