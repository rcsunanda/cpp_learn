This git repository was created to test the 'Gitflow' workflow, which involves

    - Master branch:
            For publishing final release versions.

    - Develop branch:
            For publishing and merging feature branches (branched out from master).

    - Feature branches:
            Actual features being worked on by developers (branched out from develop and merged back when done).

    - Maintanance (hotfix) branches:
            To patch/bugfix release versions in the Master branch (branched out from Master and merged back when done). Also merged on to Develop to update the codebase.
            Hotfix branches are the only kind of branch that are branched out from the Master branch apart from the Develop branch.

    - Release branches:
            When some milestone is achieved in the Develop branch and the project is ready for a release, a Release branch is created by branching out from Develop.
            Any work like polishing up the codebase and readying it for the release is done in the Release branch.
            When done, the Release branch is merged on to the Master branch and tagged with a release version.
            The Release branch is also merged backed on to the Develop branch to update the actual development code base.
            
