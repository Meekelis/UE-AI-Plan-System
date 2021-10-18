# AI Plan System

AI Plan System is a plugin for Unreal Engine 4.27. It offers a new Blueprint asset type to encapsulate AI plans for planning AI agents and provides a way to run these plans from any Blueprint through a custom K2 node.

An AI plan is a Blueprint where multiple AI actions are defined and eventually a Finish node is called.

The plugin is in beta and under development.

## Why

AI agents frequently have tasks that take time to execute. Calling latent nodes and binding to delegates can work when you are managing one AI agent. However, if you have a Blueprint manager responsible for a group of AI agents, it becomes nearly impossible to keep track of when they finish latent actions. Even if we find a way to do that, the Blueprint graphs become cluttered.

AI plans are a better solution. Each plan can be encapsulated in a separate Blueprint asset, and AI agents can execute them when requested. A plan may string many actions. And it always only deals with one AI agent. So when a latent function finishes, you know for which agent.

Plans can also encapsulate advanced behaviours required by planning AIs and keep them out of manager classes. For example, if the AI agent needs to perform an action, the plan itself can decide what steps will be taken to achieve the goal. This is a more flexible approach than hierarchical task networks, although organization is less enforced.

## Tutorial

### Creating a new AI plan

To create a new AI plan, just create a new Blueprint anywhere in the project and choose "AIPlan" as its parent.

![https://i.imgur.com/IHmBokw.png](https://i.imgur.com/IHmBokw.png)

Open the new asset to observe an empty Blueprint.

![https://i.imgur.com/HHtgA8h.png](https://i.imgur.com/HHtgA8h.png)

The key functions you are looking for can be overridden in the Functions section of "My Blueprint".

![https://i.imgur.com/ydUwyKf.png](https://i.imgur.com/ydUwyKf.png)

- On Execute will call when this plan starts executing.
- On Stop Execute will call when this plan finishes or is aborted.
- On Update will call every frame.

All of these functions will give you access to the AI controller and AI pawn for convenience.

You can create new variables and choose to have them Exposed on Spawn. This will allow you to set these variables when running the AI plan from Blueprints.

Once your plan is complete, it might look like this:

![https://i.imgur.com/KYgJXHb.png](https://i.imgur.com/KYgJXHb.png)

In this plan, the AI agent will attempt to move to a given location. If it fails, the plan fails. If it succeeds, it will play an animation, restore its health, and the plan will succeed.

### Running AI plans

Use the function "Run AI Plan" in any Blueprint to execute an AI plan. Bind to a delegate On Plan Finished in the newly created plan to call your event or delegate when the plan has finished executing.

![https://i.imgur.com/SGBOkPB.png](https://i.imgur.com/SGBOkPB.png)

Use additional functions to know if an AI is running a plan and which one:

- Get AI Plan to get the currently running AI Plan for any AI Controller.
- Is Running Plan to know whether an AI Controller is running an AI Plan.

All functions in this plugin are static for convenience. They will show up in all Blueprints. AI Plans also encapsulate latent Unreal Engine functions so you can run an AI Plan from Blueprint functions, unlike latent functions themselves.

### Configure Garbage Collection

By default, the plugin will garbage-collect AI Plans 30 seconds after they have expired. This might be unsuitable for you, depending on how often you run AI Plans.

Please go to Config/DefaultGame.ini or your overriding game config file and add the following to configure AI Plan grace period after it finishes executing:

```
[/Script/AIPlans.AIPlansManager]
LifetimeAfterExecutionStopped=30.0
```

The 30.0 default value given as example is in seconds.

You can configure different values per platform, however, generally they should be enough for your On Stop Execute events to finish their actions. 

## Download

Download the plugin in the releases section of this GitHub repository.