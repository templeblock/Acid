#pragma once

#include <thread>
#include "Maths/Time.hpp"
#include "Helpers/Delegate.hpp"

namespace acid
{
/**
 * Class that can listen to file changes on a path recursively.
 */
class ACID_EXPORT FileWatcher
{
public:
	enum class Status
	{
		Created, Modified, Erased
	};

	/**
	 * Creates a new file watcher.
	 * @param path The path to watch recursively.
	 * @param delay How frequently to check for changes.
	 */
	explicit FileWatcher(std::string path, const Time &delay = Time::Seconds(5.0f));

	~FileWatcher();

	const std::string &GetPath() const { return m_path; }

	void SetPath(const std::string &path) { m_path = path; }

	const Time &GetDelay() const { return m_delay; }

	void SetDelay(const Time &delay) { m_delay = delay; }

	/**
	 * Called when a file or directory has changed.
	 * @return The delegate.
	 */
	Delegate<void(std::string, Status)> &OnChange() { return m_onChange; }

private:
	void QueueLoop();

	bool Contains(const std::string &key) const;

	std::string m_path;
	Time m_delay;
	Delegate<void(std::string, Status)> m_onChange;

	std::thread m_thread;
	std::unordered_map<std::string, long> m_paths;
	bool m_running;
};
}
