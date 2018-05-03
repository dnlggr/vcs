# Errors and Pitfalls

## Incremental Commit IDs

At fist it sounded like a good idea to just use integers as commit IDs and always increment the last ID upon adding a new commit. 

This gives has several advantages:

- Easy and cheap ID generation
- Human readable and understandable
- Good for debugging the `changes/` directory

We decided to go for that approach. However when implementing branching we would have had to use some sort of global persisted counter to keep track of the various IDs among different branches. For simplification we then decided to use UUIDs as commit IDs since they do not require any coordination. This is similar to Git's approach of using hashes a commit IDs. the change from integers ti UUIDs cost some valuable time, however.

The UUID approach however has the disadvantage that it worsens readability and makes debugging mire difficult.

### Conclusion

 We should have though more carefully from the beginning about which ID solution would have been the best fit for our project.

## Not using `diff`s from the beginning aka being way too rapid

Our general strategy was to implement the project incrementally, meaning that we aimed at getting a stable feature set before we concentrated on optimizing said features. For instance:

 - At first, we decided to treat changed files between two commits just like newly added files. This meant that we could just store the whole file and copy it over the old file when building up the working directory for a certain commit e.g. when doing a *checkout*. Of course this was less implementation work than just storing `diff`s and patching the old files with said `diff`s.
 - We first only implemented support for the root level of the working directory without considering files inside directories etc.

However as time went on we had more and more `// todos` in our codebase and it was impossible to fix all of them.

### Conclusion

While it is certainly the right way to build a project incrementally, again, we should have spend a little more time planning what more complex things should have been implemented from the start and what was worth to postpone to later.

## Messy exception/error and edge case handling

We did not really care too much about consistent error handling which we omitted for the sake of simplicity. However, when writing tests we noticed that it is extremely hard to test expected failures if there is no coherent error handling to test against.

### Conclusion

Consistent error handling from the start!

## C++

We are C++ noobs and heavily underestimated the time it takes to read documentation and references of even the simplest things.

### Conclusion

Start earlier, have more time, profit.

## Change lists in `Commit` objects

At first it sounded very cool to have commit objects own a list of the changes they represent. We spend quite some time implementing this feature and serializing the change lists to disk. However, because changed files need to be stored on disk anyway it is pretty useless to have them duplicated in the respective commit objects.

### Conclusion

Many cool things are useless. Think more before investing a lot of time into something.