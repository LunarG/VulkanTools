# **Vulkan Tools Repository Management**

## **Open Source Project Objectives**

* Provide tools to aid in the development of Vulkan applications. Current tools include:
  * via - Vulkan Installation Analyzer
  * layers: device simulation, api dump, fps monitor, screenshot
  * layer factory: framework that facilitates the creation of new layers
* Platform support:
  * Windows
  * Linux
  * Android
* Repo Quality
  * All repo tests passing
  * Continuous Integration: HW test farms operated by Google and LunarG perform pre-commit cloud testing on pull-requests on various hardware/software platforms

## **Roles and Definitions**

* Contributor, Commenter, User
  * Submitting contributions, creating issues, or using the contents of the repository
* Approver
  * Experienced project members who have made significant technical contributions
  * Write control: Approve pull/merge requests (verify submissions vs. acceptance criteria)
* Technical Project Lead
  * Lead the project in terms of versioning, quality assurance, and overarching objectives
  * Monitor github issues, assign issues to the appropriate developer, and drive timely resolution
  * Designate new approvers
  * Ensure project documentation such as Readme, Contributing, etc. is kept up-to-date
  * Act as a facilitator in resolving technical conflicts
  * Point-of-contact for project-related questions

The technical project lead for this repository is:

* **Tony Barbour** [tony@lunarg.com](mailto:tony@lunarg.com)

## **Acceptance Criteria and Process**

* All source code to include Apache 2.0 license.
  * Additional copyrights of contributors appended
* Contributions are via pull requests
  * Project Lead will assign approvers to contributor pull requests
  * Approvers can self-assign their reviewers
  * For complex or invasive contributions,  Project Lead may request approval from specific reviewers
  * At least one review approval is required to complete a pull request
  * The goal is to be responsive to contributors while ensuring acceptance criteria is met and to facilitate their submissions
  * Approval is dependent upon adherence to the guidelines in [CONTRIBUTING.md](CONTRIBUTING.md), and alignment with
    repository goals of maintainability, completeness, and quality
  * Conflicts or questions will ultimately be resolved by the project lead
