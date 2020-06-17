## How to Contribute to the VulkanTools Repository

### **The Repository**

The source code for the VulkanTools components is sponsored by LunarG.
* [VulkanTools](https://github.com/LunarG/VulkanTools)


### **Contributions Welcome**

The Vulkan Ecosystem will benefit from your contributions to VulkanTools.  While there are often
active and organized development efforts underway to improve these tools, there are always
opportunities for anyone to help by contributing new tools or additional features to existing tools.

The easiest method for contributing is to examine the [issues list](https://github.com/LunarG/VulkanTools/issues)
and look for areas that are of interest.

Of course, if you have your own work in mind, please open an issue to describe it and assign it to yourself.
Finally, please feel free to contact any of the developers that are actively contributing should you
wish to coordinate further.

Repository Issue labels:

* _Bug_:          These issues refer to invalid or broken functionality and are the highest priority.
* _Incomplete_:   These issues refer to missing validation checks that users have encountered during application
development that would have been directly useful, and are high priority.
* _Enhancement_:  These issues refer ideas for improving or extending functionality.

It is the maintainers goal for all issues to be assigned within one business day of their submission. If you choose
to work on an issue that is assigned, simply coordinate with the current assignee.

### **How to Submit Fixes**

* **Ensure that the bug was not already reported or fixed** by searching on GitHub under Issues
  and Pull Requests.
* Use the existing GitHub forking and pull request process.
  This will involve [forking the repository](https://help.github.com/articles/fork-a-repo/),
  creating a branch with your commits, and then [submitting a pull request](https://help.github.com/articles/using-pull-requests/).
* Please read and adhere to the style and process [guidelines ](#coding-conventions-and-formatting) enumerated below.
* Please base your fixes on the master branch.  SDK branches are generally not updated except for critical fixes needed to repair an SDK release.
* The resulting Pull Request will be assigned to a repository maintainer. It is the maintainer's responsibility to ensure the Pull Request
  passes the Google/LunarG internal CI processes. Once the Pull Request has been approved and is passing internal CI, a repository maintainer
  will merge the PR.


#### **Coding Conventions and Formatting**
* Use the **[Google style guide](https://google.github.io/styleguide/cppguide.html)** for source code with the following exceptions:
    * The column limit is 132 (as opposed to the default value 80). The clang-format tool will handle this. See below.
    * The indent is 4 spaces instead of the default 2 spaces. Again, the clang-format tool will handle this.
    * If you can justify a reason for violating a rule in the guidelines, then you are free to do so. Be prepared to defend your
decision during code review. This should be used responsibly. An example of a bad reason is "I don't like that rule." An example of
a good reason is "This violates the style guide, but it improves type safety."

* Run **clang-format** on your changes to maintain consistent formatting
    * There are `.clang-format files` present in the repository to define clang-format settings
      which are found and used automatically by clang-format.
    * A sample git workflow may look like:

>        # Make changes to the source.
>        $ git add -u .
>        $ git clang-format --style=file
>        # Check to see if clang-format made any changes and if they are OK.
>        $ git add -u .
>        $ git commit

* **Commit Messages**
    * Limit the subject line to 50 characters -- this allows the information to display correctly in git/Github logs
    * Begin subject line with a one-word component description followed by a colon (e.g. layer/tool name, tests, etc.)
    * Separate subject from body with a blank line
    * Wrap the body at 72 characters
    * Capitalize the subject line
    * Do not end the subject line with a period
    * Use the body to explain what and why vs. how
    * Use the imperative mode in the subject line. This just means to write it as a command (e.g. Fix the sprocket)

Strive for commits that implement a single or related set of functionality, using as many commits as is necessary (more is better).
That said, please ensure that the repository compiles and passes tests without error for each commit in your pull request.  Note
that to be accepted into the repository, the pull request must [pass all tests](#testing your changes) on all supported platforms
-- the automatic Github Travis and AppVeyor continuous integration features will assist in enforcing this requirement.

#### **Testing Your Changes**
* Run the existing tests in the repository before and after each of your commits to check for any regressions.
  The tests can be found in the test directory inside of your target build directory
* Note that some tests may fail with known issues or driver-specific problems.
  The idea here is that your changes should not change the test results, unless that was the intent of your changes.
* Run tests that explicitly exercise your changes.
* Feel free to subject your code changes to other tests as well!


### **Contributor License Agreement (CLA)**

You will be prompted with a one-time "click-through" CLA dialog as part of submitting your pull request
or other contribution to GitHub.

### **License and Copyrights**

All contributions made to the LunarG repositories need to have the Apache 2.0 license.
Please see an existing file in this repository for an example.

You can include your individual copyright after any existing copyrights.
